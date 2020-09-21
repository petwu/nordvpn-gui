#include "statuscontroller.h"

bool StatusController::canExecuteShellCmds() {
    auto result = execute("");
    return *result != *ERROR_POPEN_FAILED;
}

bool StatusController::isNordVpnInstalled() {
    auto result = execute(nordvpn::CMD_VERSION);
    return result->exitCode == 0 &&
           result->output.rfind("NordVPN Version", 0) == 0;
}

std::string StatusController::getVersion() {
    auto result = execute(nordvpn::CMD_VERSION);
    return result->output;
}

std::unique_ptr<ConnectionStatus> StatusController::getStatus() {
    auto result = execute(nordvpn::CMD_STATUS);
    auto status = std::unique_ptr<ConnectionStatus>(new ConnectionStatus());
    auto o = result->output;
    std::smatch m;
    bool matched;

    // connection status
    matched = std::regex_search(o, m, std::regex("Status: (\\w+)"));
    if (!matched || m[1].str() == "Disconnected") {
        return status;
    }
    status->connected = true;

    // server
    matched = std::regex_search(o, m, std::regex("Current server: (.+)"));
    if (matched)
        status->server = m[1].str();

    // country
    matched = std::regex_search(o, m, std::regex("Country: (.+)"));
    if (matched)
        status->country = m[1].str();

    // city
    matched = std::regex_search(o, m, std::regex("City: (.+)"));
    if (matched)
        status->city = m[1].str();

    // ip
    matched = std::regex_search(o, m, std::regex("Your new IP: (.+)"));
    if (matched)
        status->ip = m[1].str();

    // technology
    matched = std::regex_search(o, m, std::regex("Current technology: (.+)"));
    if (matched)
        status->technology = Technology::fromString(m[1].str());

    // connection type
    matched = std::regex_search(o, m, std::regex("Current protocol: (.+)"));
    if (matched)
        status->connectionType = ConnectionType::fromString(m[1].str());

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
    if (matched)
        status->sent =
            uint64_t(std::atof(m[1].str().c_str()) * bytes[m[2].str()]);

    // received
    matched = std::regex_search(
        o, m, std::regex("([\\d\\.]+) (B|KiB|MiB|GiB|TiB) received"));
    if (matched)
        status->received =
            uint64_t(std::atof(m[1].str().c_str()) * bytes[m[2].str()]);

    // uptime
    matched = std::regex_search(
        o, m,
        std::regex("Uptime: ?((\\d+) years?)? ?((\\d+) days?)? ?((\\d+) "
                   "hours?)? ?((\\d+) minutes?)? ?((\\d+) seconds?)?"));
    if (matched)
        status->uptime = std::atoi(m[10].str().c_str()) +     // seconds
                         (std::atoi(m[8].str().c_str()) +     // minutes
                          (std::atoi(m[6].str().c_str()) +    // hours
                           (std::atoi(m[4].str().c_str()) +   // days
                            (std::atoi(m[2].str().c_str())) * // years
                                365) *                        // years -> days
                               24) *                          // days -> hours
                              60) * // hours -> minutes
                             60;    // minutes -> seconds

    return status;
}

uint8_t StatusController::getRatingMin() { return nordvpn::RATING_MIN; }

uint8_t StatusController::getRatingMax() { return nordvpn::RADING_MAX; }

void StatusController::rate(uint8_t rating) {
    if (rating < getRatingMin() || rating > getRatingMax()) {
        return;
    }
    execute(nordvpn::CMD_RATE + " " + std::to_string(rating));
}
