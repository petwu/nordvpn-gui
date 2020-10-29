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

    // protocol
    if (std::regex_search(o, m, std::regex("Protocol: (\\w+)")))
        s.setProtocol(protocolFromString(m[1].str()));

    // technology
    if (std::regex_search(o, m, std::regex("Technology: (\\w+)")))
        s.setTechnology(technologyFromString(m[1].str()));

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

    std::thread([cmds] {
        for (auto cmd : cmds)
            Process::execute(cmd);
    }).detach();
}

NordVpnSettings PreferencesController::restoreDefaultNordvpnSettings() {
    Process::execute("nordvpn set defaults");
    return this->getNordvpnSettings();
}
