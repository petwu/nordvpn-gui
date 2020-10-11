#include "nordvpnsettings.h"

std::string NordVpnSettings::toJSON() {
    json j;
    j["autoconnect"] = this->autoconnect;
    j["cybersec"] = this->cybersec;
    j["dns"] = this->dns;
    for (auto address : this->dnsAddresses)
        j["dnsAddresses"].push_back(address);
    j["killswitch"] = this->killswitch;
    j["notify"] = this->notify;
    j["obfuscated"] = this->obfuscated;
    j["protocol"] = protocolToString(this->protocol);
    j["technology"] = technologyToString(this->technology);
    return std::move(j.dump());
}

NordVpnSettings NordVpnSettings::fromJSON(std::string settingsJson) {
    json j = json::parse(settingsJson);
    NordVpnSettings s;
    if (j["autoconnect"].is_boolean())
        s.autoconnect = json::boolean_t(j["autoconnect"]);
    if (j["cybersec"].is_boolean())
        s.cybersec = json::boolean_t(j["cybersec"]);
    if (j["dns"].is_boolean())
        s.dns = json::boolean_t(j["dns"]);
    if (j["dnsAddresses"].is_array()) {
        int i = 0;
        int n = sizeof(s.dnsAddresses) / sizeof(s.dnsAddresses[0]);
        for (auto address : j["dnsAddresses"]) {
            if (i == n)
                break;
            if (address.is_string())
                s.dnsAddresses[i] = json::string_t(address);
            i++;
        }
    }
    if (j["killswitch"].is_boolean())
        s.killswitch = json::boolean_t(j["killswitch"]);
    if (j["notify"].is_boolean())
        s.notify = json::boolean_t(j["notify"]);
    if (j["obfuscated"].is_boolean())
        s.obfuscated = json::boolean_t(j["obfuscated"]);
    if (j["protocol"].is_string())
        s.protocol = protocolFromString(json::string_t(j["protocol"]));
    if (j["technology"].is_string())
        s.technology = technologyFromString(json::string_t(j["technology"]));
    return std::move(s);
}
