#include "statuscontroller.h"

StatusController::StatusController() {
    this->_countries = ServerController{}.getAllCountries();
}

std::string ConnectionInfo::toString(bool onLine) const {
    std::string sb = onLine ? " " : "\n  ";
    std::string s = onLine ? ", " : "\n  ";
    std::string se = onLine ? " " : "\n";
    return std::string("ConnectionInfo {") + sb +
           "status = " + this->status.toString() + s +
           "server = " + this->server + s +
           "serverId = " + std::to_string(this->serverId) + s +
           "country = " + this->country + s +
           "countryId = " + std::to_string(this->countryId) + s +
           "city = " + this->city + s + "ip = " + this->ip + s +
           "technology = " + this->technology.toString() + s +
           "connectionType = " + this->connectionType.toString() + s +
           "sent = " + std::to_string(this->sent) + s +
           "received = " + std::to_string(this->received) + s +
           "uptime = " + std::to_string(this->uptime) + "s" + se + "}";
}

bool StatusController::canExecuteShellCmds() {
    auto result = this->execute("");
    return result != ERROR_POPEN_FAILED;
}

bool StatusController::isNordVpnInstalled() {
    auto result = this->execute(config::cmd::VERSION);
    return result.exitCode == 0 &&
           result.output.rfind("NordVPN Version", 0) == 0;
}

std::string StatusController::getVersion() {
    auto result = this->execute(config::cmd::VERSION);
    return result.output;
}

ConnectionInfo StatusController::getStatus() {
    std::string o = this->execute(config::cmd::STATUS).output;
    ConnectionInfo info;
    std::smatch m;
    bool matched;

    // connection status => disconnected ?
    matched = std::regex_search(o, m, std::regex("Status: (\\w+)"));
    if (!matched || m[1].str() == "Disconnected") {
        return info;
    }

    // server
    matched = std::regex_search(o, m, std::regex("Current server: (.+)"));
    if (matched) {
        info.server = m[1].str();
        info.serverId = this->_getServerId(info.server);
    }

    // country
    matched = std::regex_search(o, m, std::regex("Country: (.+)"));
    if (matched) {
        info.country = m[1].str();
        info.countryId = this->_getCountryId(info.country);
    }

    // city
    matched = std::regex_search(o, m, std::regex("City: (.+)"));
    if (matched) {
        info.city = m[1].str();
    }

    // ip
    matched = std::regex_search(o, m, std::regex("Your new IP: (.+)"));
    if (matched) {
        info.ip = m[1].str();
    }

    // technology
    matched = std::regex_search(o, m, std::regex("Current technology: (.+)"));
    if (matched) {
        info.technology = Technology::fromString(m[1].str());
    }

    // connection type
    matched = std::regex_search(o, m, std::regex("Current protocol: (.+)"));
    if (matched) {
        info.connectionType = ConnectionType::fromString(m[1].str());
    }

    std::map<std::string, uint64_t> bytes = {
        {"B", 1},
        {"KiB", 1024},
        {"MiB", 1048576},
        {"GiB", 1073741824},
        {"TiB", 1099511627776},
    };
    // sent
    matched = std::regex_search(
        o, m, std::regex("([\\d\\.]+) (B|KiB|MiB|GiB|TiB) sent"));
    if (matched) {
        info.sent = uint64_t(std::atof(m[1].str().c_str()) * bytes[m[2].str()]);
    }

    // received
    matched = std::regex_search(
        o, m, std::regex("([\\d\\.]+) (B|KiB|MiB|GiB|TiB) received"));
    if (matched) {
        info.received =
            uint64_t(std::atof(m[1].str().c_str()) * bytes[m[2].str()]);
    }

    // uptime
    matched = std::regex_search(
        o, m,
        std::regex("Uptime: ?((\\d+) years?)? ?((\\d+) days?)? ?((\\d+) "
                   "hours?)? ?((\\d+) minutes?)? ?((\\d+) seconds?)?"));
    if (matched) {
        info.uptime = std::atoi(m[10].str().c_str()) +     // seconds
                      (std::atoi(m[8].str().c_str()) +     // minutes
                       (std::atoi(m[6].str().c_str()) +    // hours
                        (std::atoi(m[4].str().c_str()) +   // days
                         (std::atoi(m[2].str().c_str())) * // years
                             365) *                        // years -> days
                            24) *                          // days -> hours
                           60) *                           // hours -> minutes
                          60;                              // minutes -> seconds
    }

    // connection status => connecting or connected ?
    info.status = info.sent == 0 || info.uptime == 0
                      ? ConnectionStatus::Connecting
                      : ConnectionStatus::Connected;

    return std::move(info);
}

int32_t StatusController::_getCountryId(std::string name) {
    json countries = this->_countries;
    for (auto i = countries.begin(); i != countries.end(); ++i) {
        json country = i.value();
        if (country.is_object() && country["id"].is_number_integer() &&
            (country["statusName"].is_string() ||
             country["connectName"].is_string())) {
            std::string sName = util::string::toLower(
                country["statusName"].is_string() ? country["statusName"] : "");
            std::string cName = util::string::toLower(
                country["connectName"].is_string() ? country["connectName"]
                                                   : "");
            std::string n1 = util::string::toLower(name);
            std::string n2 = util::string::replaceAll(n1, "_", " ");
            if (sName == n1 || sName == n2 || cName == n1 || cName == n2) {
                return country["id"];
            }
        }
    }
    return -1;
}

int32_t StatusController::_getServerId(std::string name) {
    std::smatch m;
    if (std::regex_search(name, m, std::regex("^([a-zA-Z]+)(\\d+)\\..+"))) {
        return std::atoi(m[2].str().c_str());
    }
    return 0;
}

void StatusController::startBackgroundTask() {
    this->_performBackgroundTask = true;
    // create and run new daemon thread
    std::thread t(&StatusController::_backgroundTask, this);
    t.detach();
}

void StatusController::stopBackgroundTask() {
    this->_performBackgroundTask = false;
}

void StatusController::attach(IConnectionInfoSubscription *subscriber) {
    this->_subscribers.push_back(subscriber);
}

void StatusController::detach(IConnectionInfoSubscription *subscriber) {
    this->_subscribers.erase(std::remove(this->_subscribers.begin(),
                                         this->_subscribers.end(), subscriber),
                             this->_subscribers.end());
}

void StatusController::_backgroundTask() {
    while (this->_performBackgroundTask) {
        this->_currectStatus = this->getStatus();
        this->_notifySubscribers();
        std::this_thread::sleep_for(config::consts::STATUS_UPDATE_INTERVAL);
    }
}

void StatusController::_notifySubscribers() {
    for (auto &subscriber : this->_subscribers) {
        subscriber->update(this->_currectStatus);
    }
}

uint8_t StatusController::getRatingMin() { return config::consts::RATING_MIN; }

uint8_t StatusController::getRatingMax() { return config::consts::RATING_MAX; }

void StatusController::rate(uint8_t rating) {
    return;
    if (rating < getRatingMin() || rating > getRatingMax()) {
        return;
    }
    this->executeNonBlocking(config::cmd::RATE + " " + std::to_string(rating));
}
