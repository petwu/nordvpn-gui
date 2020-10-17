#include "settingscontroller.h"

const std::string ENABLED = "enabled";
const std::string DISABLED = "disabled";

std::string boolToEnabledString(bool enabled) {
    return enabled ? ENABLED : DISABLED;
}

SettingsController &SettingsController::getInstance() {
    static SettingsController instance;
    return instance;
}

NordVpnSettings SettingsController::getSettings() {
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
        s.autoconnect = (m[1].str() == ENABLED);

    // cybersec
    if (std::regex_search(o, m, std::regex("CyberSec: " + enOrDisabled)))
        s.cybersec = (m[1].str() == ENABLED);

    // dns
    s.dns = !std::regex_search(o, m, std::regex("DNS: disabled"));
    if (s.dns) { // dns not disabled => dns enabled
        if (std::regex_search(o, m, std::regex("DNS: ((.+, )*.+)"))) {
            int i = 0;
            int n = sizeof(s.dnsAddresses) / sizeof(s.dnsAddresses[0]);
            for (auto ip : util::string::split(m[1].str(), ", ")) {
                if (i == n)
                    break;
                s.dnsAddresses[i] = ip;
                i++;
            }
        }
    }

    // kill switch
    if (std::regex_search(o, m, std::regex("Kill Switch: " + enOrDisabled)))
        s.killswitch = (m[1].str() == ENABLED);

    // notify
    if (std::regex_search(o, m, std::regex("Notify: " + enOrDisabled)))
        s.notify = (m[1].str() == ENABLED);

    // obfuscated
    if (std::regex_search(o, m, std::regex("Obfuscate: " + enOrDisabled)))
        s.obfuscated = (m[1].str() == ENABLED);

    // protocol
    if (std::regex_search(o, m, std::regex("Protocol: (\\w+)")))
        s.protocol = protocolFromString(m[1].str());

    // technology
    if (std::regex_search(o, m, std::regex("Technology: (\\w+)")))
        s.technology = technologyFromString(m[1].str());

    return std::move(s);
}

void SettingsController::updateSettings(const NordVpnSettings &settings) {
    // change a settings value only if it differs from the current value
    auto current = this->getSettings();

    // autoconnect
    if (settings.autoconnect != current.autoconnect)
        AsyncProcess::execute("nordvpn set autoconnect " +
                              boolToEnabledString(settings.autoconnect));

    // cybersec
    if (settings.cybersec != current.cybersec)
        AsyncProcess::execute("nordvpn set cybersec " +
                              boolToEnabledString(settings.cybersec));

    // dns
    if (settings.dns != current.dns && settings.dns == false)
        AsyncProcess::execute("nordvpn set dns " + DISABLED);
    if (settings.dns == true) {
        bool equal = true;
        std::string joinedIPs;
        for (int i = 0; i < sizeof(settings.dnsAddresses) /
                                sizeof(settings.dnsAddresses[0]);
             i++) {
            if (util::string::trim(settings.dnsAddresses[i]) != "")
                joinedIPs += " " + settings.dnsAddresses[i];
            if (settings.dnsAddresses[i] != current.dnsAddresses[i])
                equal = false;
        }
        if (!equal) {
            if (joinedIPs != "")
                AsyncProcess::execute("nordvpn set dns" + joinedIPs);
            else
                AsyncProcess::execute("nordvpn set dns " + DISABLED);
        }
    }

    // killswitch
    if (settings.killswitch != current.killswitch)
        AsyncProcess::execute("nordvpn set killswitch " +
                              boolToEnabledString(settings.killswitch));

    // notify
    if (settings.notify != current.notify)
        AsyncProcess::execute("nordvpn set notify " +
                              boolToEnabledString(settings.notify));

    // obfuscated
    if (settings.obfuscated != current.obfuscated)
        AsyncProcess::execute("nordvpn set obfuscated " +
                              boolToEnabledString(settings.obfuscated));

    // protocol
    if (settings.protocol != current.protocol)
        AsyncProcess::execute("nordvpn set protocol " +
                              protocolToString(settings.protocol));

    // technology
    if (settings.technology != current.technology)
        AsyncProcess::execute("nordvpn set technology " +
                              technologyToString(settings.technology));
}
