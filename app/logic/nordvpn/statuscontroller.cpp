#include "statuscontroller.h"

bool ConnectionInfo::isEmpty() const {
    return this->status == ConnectionStatus::Disconnected &&
           this->server.empty() && this->serverId == 0 &&
           this->country.empty() && this->countryId == -1 &&
           this->city.empty() && this->ip.empty() &&
           this->technology == Technology::Undefined &&
           this->connectionType == Protocol::Undefined && this->sent == 0 &&
           this->received == 0 && this->uptime == 0;
}

StatusController::StatusController() {
    this->_countries = ServerController::getInstance().getAllCountries();
}

StatusController &StatusController::getInstance() {
    static StatusController instance;
    return instance;
}

bool StatusController::canExecuteShellCmds() {
    auto result = this->execute("");
    return result != ERROR_POPEN_FAILED;
}

bool StatusController::isNordVpnInstalled() {
    auto result = this->execute("nordvpn --version");
    return result.exitCode == 0 &&
           result.output.rfind("NordVPN Version", 0) == 0;
}

std::string StatusController::getVersion() {
    auto result = this->execute("nordvpn --version");
    return result.output;
}

ConnectionInfo StatusController::getStatus() {
    std::string o = this->execute("nordvpn status").output;
    ConnectionInfo info;
    std::smatch m;
    bool matched;

    // connection status => disconnected ?
    matched = std::regex_search(o, m, std::regex("Status: (\\w+)"));
    if (!matched || m[1].str() == "Disconnected") {
        this->_currentConnectedInfo = std::move(ConnectionInfo(info));
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
        info.technology = technologyFromString(m[1].str());
    }

    // connection type
    matched = std::regex_search(o, m, std::regex("Current protocol: (.+)"));
    if (matched) {
        info.connectionType = protocolFromString(m[1].str());
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
    matched = std::regex_search(o, m, std::regex("Status: (\\w+)"));
    if (matched) {
        if (m[1].str() == "Connecting" || m[1].str() == "Reconnecting") {
            info.status = ConnectionStatus::Connecting;
        } else if (m[1].str() == "Connected") {
            if (info.sent == 0 ||
                (!this->_currentConnectedInfo.isEmpty() &&
                 this->_currentConnectedInfo.server != info.server &&
                 info.uptime >= this->_currentConnectedInfo.uptime)) {
                /*
                 * [ special case ]
                 *
                 * Cause:
                 * While switching the connected country, the NordVPN CLI
                 * returns an inconsistent status: During the connecting phase,
                 * the properties "Current server", "Country", "City" and "Your
                 * new IP" already have the values of the new country while
                 * "Status", "Transfer" and "Uptime" still have the values of
                 * the old country. Thus the "Status" still states "Connected"
                 * though it should be "Connecting".
                 *
                 * Detection:
                 * This case can be detected by checking whether the previously
                 * remmembered connected server (_currentConnectedInfo) matches
                 * the server of the currently parsed status info. If yes: We
                 * are still connected to the same server. If no: We are
                 * switching servers and thus the "Status" property is wrong;
                 * the "Uptime" property is wrong as well. In some cases, the
                 * sent bytes ("Transfer") also already resetted to 0.
                 *
                 */
                info.status = ConnectionStatus::Connecting;
                info.sent = 0;
            } else {
                info.status = ConnectionStatus::Connected;
                this->_currentConnectedInfo = std::move(ConnectionInfo(info));
            }
        }
    }

    return std::move(info);
}

int32_t StatusController::_getCountryId(std::string name) {
    std::vector<Country> countries = this->_countries;
    for (auto i = 0; i < countries.size(); i++) {
        auto country = countries[i];
        std::string sName = util::string::toLower(country.name);
        std::string cName = util::string::toLower(country.connectName);
        std::string n1 = util::string::toLower(name);
        std::string n2 = util::string::replaceAll(n1, "_", " ");
        if (sName == n1 || sName == n2 || cName == n1 || cName == n2) {
            return country.id;
        }
    }
    return -1;
}

int32_t StatusController::_getServerId(std::string name) {
    std::smatch m;
    if (std::regex_search(name, m, std::regex("^([a-zA-Z-]+)(\\d+)\\..+"))) {
        return std::atoi(m[2].str().c_str());
    }
    return 0;
}

void StatusController::startBackgroundTask() {
    this->_performBackgroundTask = true;
    // create and run new daemon thread
    std::thread(&StatusController::_backgroundTask, this).detach();
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
        this->_currentInfo = this->getStatus();
        this->_notifySubscribers();
        std::this_thread::sleep_for(config::consts::STATUS_UPDATE_INTERVAL);
    }
}

void StatusController::_notifySubscribers() {
    for (auto &subscriber : this->_subscribers) {
        subscriber->updateConnectionInfo(this->_currentInfo);
    }
}

uint8_t StatusController::getRatingMin() { return config::consts::RATING_MIN; }

uint8_t StatusController::getRatingMax() { return config::consts::RATING_MAX; }

void StatusController::rate(uint8_t rating) {
    return;
    if (rating < getRatingMin() || rating > getRatingMax()) {
        return;
    }
    this->executeNonBlocking("nordvpn rate " + std::to_string(rating));
}
