#include "statuscontroller.h"

auto StatusController::getInstance() -> StatusController & {
    static StatusController instance;
    return instance;
}

auto StatusController::getStatus() -> ConnectionInfo {
    std::string o = Process::execute("nordvpn status").output;
    ConnectionInfo info;
    std::smatch m;
    bool matched = false;

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
        info.serverNr = StatusController::_getServerNr(info.server);
    }

    // country
    matched = std::regex_search(o, m, std::regex("Country: (.+)"));
    if (matched) {
        info.country = m[1].str();
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
        {"B", Byte}, {"KiB", KiB}, {"MiB", MiB}, {"GiB", GiB}, {"TiB", TiB},
    };
    // sent
    matched = std::regex_search(
        o, m, std::regex("([\\d\\.]+) (B|KiB|MiB|GiB|TiB) sent"));
    if (matched) {
        info.sent = uint64_t(std::strtod(m[1].str().c_str(), nullptr) *
                             bytes[m[2].str()]);
    }

    // received
    matched = std::regex_search(
        o, m, std::regex("([\\d\\.]+) (B|KiB|MiB|GiB|TiB) received"));
    if (matched) {
        info.received = uint64_t(std::strtod(m[1].str().c_str(), nullptr) *
                                 bytes[m[2].str()]);
    }

    // uptime
    matched = std::regex_search(
        o, m,
        std::regex("Uptime: ?((\\d+) years?)? ?((\\d+) days?)? ?((\\d+) "
                   "hours?)? ?((\\d+) minutes?)? ?((\\d+) seconds?)?"));
    if (matched) {
        info.uptime =
            std::strtol(m[10].str().c_str(), nullptr, 10) +     // seconds
            (std::strtol(m[8].str().c_str(), nullptr, 10) +     // minutes
             (std::strtol(m[6].str().c_str(), nullptr, 10) +    // hours
              (std::strtol(m[4].str().c_str(), nullptr, 10) +   // days
               (std::strtol(m[2].str().c_str(), nullptr, 10)) * // years
                   365) *                                       // years -> days
                  24) *                                         // days -> hours
                 60) *                                          // hours -> mins
                60;                                             // mins -> secs
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

    // additonal server information
    Server server =
        ServerController::getInstance().getServerByHostname(info.server);
    info.groups = server.groups;
    info.load = server.load;
    info.countryId = server.countryId;
    info.cityId = server.cityId;

    return std::move(info);
}

auto StatusController::_getServerNr(const std::string &name) -> int32_t {
    std::smatch m;
    if (std::regex_search(name, m, std::regex("^([a-zA-Z-]+)(\\d+)\\..+"))) {
        return std::strtol(m[2].str().c_str(), nullptr, 10);
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

auto StatusController::getRatingMin() -> uint8_t {
    return config::consts::RATING_MIN;
}

auto StatusController::getRatingMax() -> uint8_t {
    return config::consts::RATING_MAX;
}

void StatusController::rate(uint8_t rating) {
    if (rating < getRatingMin() || rating > getRatingMax()) {
        return;
    }
    AsyncProcess::execute("nordvpn rate " + std::to_string(rating));
}
