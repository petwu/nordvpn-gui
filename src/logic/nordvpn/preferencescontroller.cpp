#include "preferencescontroller.h"

const std::string ENABLED = "enabled";
const std::string DISABLED = "disabled";

std::string boolToEnabledString(bool enabled) {
    return enabled ? ENABLED : DISABLED;
}

PreferencesController &PreferencesController::getInstance() {
    static PreferencesController instance;
    return instance;
}

NordVpnSettings PreferencesController::getNordvpnSettings() {
    NordVpnSettings s;
    // get current settings from NordVPN CLI
    auto result = Process::execute("nordvpn settings");
    if (result.exitCode != 0)
        return std::move(s);

    // parse the output with regexes
    std::string o = result.output;
    std::smatch m;
    // regex to detect either enabled or disabled
    const std::string enOrDisabled = "(" + ENABLED + "|" + DISABLED + ")";

    // technology
    if (std::regex_search(o, m, std::regex("Technology: (\\w+)")))
        s.setTechnology(technologyFromString(m[1].str()));

    // protocol
    if (std::regex_search(o, m, std::regex("Protocol: (\\w+)")))
        s.setProtocol(protocolFromString(m[1].str()));

    // auto connect
    if (std::regex_search(o, m, std::regex("Auto-connect: " + enOrDisabled)))
        s.setAutoconnect(m[1].str() == ENABLED);

    // cybersec
    if (std::regex_search(o, m, std::regex("CyberSec: " + enOrDisabled)))
        s.setCybersec(m[1].str() == ENABLED);

    // dns
    s.setDns(!std::regex_search(o, m, std::regex("DNS: disabled")));
    if (s.getDns()) { // dns not disabled => dns enabled
        if (std::regex_search(o, m, std::regex("DNS: ((.+, )*.+)"))) {
            int i = 0;
            int n = s.getMaxNumberOfDnsAddresses();
            for (auto ip : util::string::split(m[1].str(), ", ")) {
                if (i == n)
                    break;
                s.setDnsAddress(i, ip);
                i++;
            }
        }
    }

    // kill switch
    if (std::regex_search(o, m, std::regex("Kill Switch: " + enOrDisabled)))
        s.setKillswitch(m[1].str() == ENABLED);

    // notify
    if (std::regex_search(o, m, std::regex("Notify: " + enOrDisabled)))
        s.setNotify(m[1].str() == ENABLED);

    // obfuscated
    if (std::regex_search(o, m, std::regex("Obfuscate: " + enOrDisabled)))
        s.setObfuscated(m[1].str() == ENABLED);

    // whitelist
    bool inWhitelistPorts = false;
    bool inWhitelistSubnets = false;
    for (auto line : util::string::split(o, "\n")) {
        line = util::string::trim(line);
        if (line == "Whitelisted ports:") {
            inWhitelistPorts = true;
            inWhitelistSubnets = false;
            continue;
        } else if (line == "Whitelisted subnets:") {
            inWhitelistPorts = false;
            inWhitelistSubnets = true;
            continue;
        }
        if (inWhitelistPorts) {
            if (std::regex_search(
                    line, m,
                    std::regex("(\\d+)(\\s*-\\s*(\\d+))?\\s+\\((.+)\\)"))) {
                auto from = m[1].str();
                auto to = m[3].str();
                auto protocols = m[4].str();
                WhitelistPortEntry whiteEntry;
                whiteEntry.portFrom = std::stoi(from);
                if (to != "")
                    whiteEntry.portTo = std::stoi(to);
                else
                    whiteEntry.portTo = whiteEntry.portFrom;
                if (protocols == "TCP") {
                    whiteEntry.protocolFlag = Protocol::TCP;
                } else if (protocols == "UDP") {
                    whiteEntry.protocolFlag = Protocol::UDP;
                } else if (protocols == "TCP|UDP" || protocols == "UDP|TCP") {
                    whiteEntry.protocolFlag = Protocol::UDP | Protocol::TCP;
                }
                s.addPortsToWhitelist(whiteEntry);
            }
        } else if (inWhitelistSubnets) {
            s.addSubnetToWhitelist(line);
        }
    }

    return std::move(s);
}

void PreferencesController::updateNordvpnSettings(
    const NordVpnSettings &settings) {
    // change a settings value only if it differs from the current value
    auto current = this->getNordvpnSettings();
    std::vector<std::string> cmds;

    // technology
    if (settings.getTechnology() != current.getTechnology())
        cmds.push_back("nordvpn set technology " +
                       technologyToString(settings.getTechnology()));

    // protocol
    if (settings.getProtocol().isNotNull() &&
        settings.getProtocol() != current.getProtocol())
        cmds.push_back("nordvpn set protocol " +
                       protocolToString(settings.getProtocol().value()));

    // autoconnect
    if (settings.getAutoconnect() != current.getAutoconnect())
        cmds.push_back("nordvpn set autoconnect " +
                       boolToEnabledString(settings.getAutoconnect()));

    // cybersec
    if (settings.getCybersec() != current.getCybersec())
        cmds.push_back("nordvpn set cybersec " +
                       boolToEnabledString(settings.getCybersec()));

    // dns
    if (settings.getDns() != current.getDns() && settings.getDns() == false)
        cmds.push_back("nordvpn set dns " + DISABLED);
    if (settings.getDns() == true) {
        bool equal = true;
        std::string joinedIPs;
        auto dnsAddresses = settings.getDnsAddresses();
        for (int i = 0; i < settings.getMaxNumberOfDnsAddresses(); i++) {
            if (util::string::trim(dnsAddresses[i]) != "")
                joinedIPs += " " + dnsAddresses[i];
            if (dnsAddresses[i] != current.getDnsAddresses()[i])
                equal = false;
        }
        if (!equal) {
            if (joinedIPs != "")
                cmds.push_back("nordvpn set dns" + joinedIPs);
            else
                cmds.push_back("nordvpn set dns " + DISABLED);
        }
    }

    // killswitch
    if (settings.getKillswitch() != current.getKillswitch())
        cmds.push_back("nordvpn set killswitch " +
                       boolToEnabledString(settings.getKillswitch()));

    // notify
    if (settings.getNotify() != current.getNotify())
        cmds.push_back("nordvpn set notify " +
                       boolToEnabledString(settings.getNotify()));

    // obfuscated
    if (settings.getObfuscated().isNotNull() &&
        settings.getObfuscated() != current.getObfuscated())
        cmds.push_back("nordvpn set obfuscate " +
                       boolToEnabledString(settings.getObfuscated().value()));

    // whitelist subnets
    auto settingsSubnets = settings.getWhitelistSubnets();
    auto currentSubnets = current.getWhitelistSubnets();
    for (auto subnet : settingsSubnets)
        // find new subnets
        if (std::find(currentSubnets.begin(), currentSubnets.end(), subnet) ==
            currentSubnets.end())
            cmds.push_back("nordvpn whitelist add subnet " + subnet);
    for (auto subnet : currentSubnets)
        // find removed subnets
        if (std::find(settingsSubnets.begin(), settingsSubnets.end(), subnet) ==
            settingsSubnets.end())
            cmds.push_back("nordvpn whitelist remove subnet " + subnet);

    // whitelist ports
    auto settingsPorts = settings.getWhitelistPorts();
    auto currentPorts = current.getWhitelistPorts();
    for (auto port : settingsPorts)
        if (std::find(currentPorts.begin(), currentPorts.end(), port) ==
            currentPorts.end())
            // find new ports
            cmds.push_back(
                "nordvpn whitelist add " +
                (port.portFrom == port.portTo
                     ? "port " + std::to_string(port.portFrom)    // single port
                     : "ports " + std::to_string(port.portFrom) + // port range
                           " " + std::to_string(port.portTo)) +
                (port.protocolFlag == Protocol::UDP
                     ? " protocol UDP" // only UDP port
                 : port.protocolFlag == Protocol::TCP
                     ? " protocol TCP" // only TCP port
                     : "")             // default is both protocols
            );
    for (auto port : currentPorts)
        if (std::find(settingsPorts.begin(), settingsPorts.end(), port) ==
            settingsPorts.end())
            // find removed ports
            cmds.push_back(
                "nordvpn whitelist remove " +
                (port.portFrom == port.portTo
                     ? "port " + std::to_string(port.portFrom)    // single port
                     : "ports " + std::to_string(port.portFrom) + // port range
                           " " + std::to_string(port.portTo)) +
                (port.protocolFlag == Protocol::UDP
                     ? " protocol UDP" // only UDP port
                 : port.protocolFlag == Protocol::TCP
                     ? " protocol TCP" // only TCP port
                     : "")             // default is both protocols
            );

    std::thread([cmds] {
        for (auto cmd : cmds)
            Process::execute(cmd);
    }).detach();
}

NordVpnSettings PreferencesController::restoreDefaultNordvpnSettings() {
    Process::execute("nordvpn set defaults");
    return this->getNordvpnSettings();
}
