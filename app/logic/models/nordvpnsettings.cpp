#include "nordvpnsettings.h"

bool NordVpnSettings::getAutoconnect() const { //
    return this->autoconnect;
}

void NordVpnSettings::setAutoconnect(bool enabled) { //
    this->autoconnect = enabled;
}

bool NordVpnSettings::getCybersec() const { //
    return this->cybersec;
}

void NordVpnSettings::setCybersec(bool enabled) { //
    this->cybersec = enabled;
    if (enabled)
        this->dns = false;
}

bool NordVpnSettings::getDns() const { //
    return this->dns;
}

void NordVpnSettings::setDns(bool enabled) {
    this->dns = enabled;
    if (enabled)
        this->cybersec = false;
}

std::vector<std::string> NordVpnSettings::getDnsAddresses() const {
    return this->dnsAddresses;
}

int NordVpnSettings::getMaxNumberOfDnsAddresses() const {
    return this->dnsAddresses.size();
}

void NordVpnSettings::setDnsAddresses(std::string addrs[3]) {
    bool allEmpty = true;
    for (int i = 0; i < 3 && i < this->dnsAddresses.size(); i++) {
        this->dnsAddresses[i] = addrs[i];
        if (addrs[i] != "")
            allEmpty = false;
    }
    this->dns = !allEmpty;
}

void NordVpnSettings::setDnsAddress(int nth, std::string addr) {
    if (nth >= this->dnsAddresses.size() || addr == "")
        return;
    this->dnsAddresses[nth] = addr;
}

bool NordVpnSettings::getKillswitch() const { //
    return this->killswitch;
}

void NordVpnSettings::setKillswitch(bool enabled) { //
    this->killswitch = enabled;
}

bool NordVpnSettings::getNotify() const { //
    return this->notify;
}

void NordVpnSettings::setNotify(bool enabled) { //
    this->notify = enabled;
}

Nullable<bool> NordVpnSettings::getObfuscated() const { //
    return this->obfuscated;
}

void NordVpnSettings::setObfuscated(bool enabled) {
    this->obfuscated = enabled;
    if (enabled)
        this->dns = false;
}

Nullable<Protocol> NordVpnSettings::getProtocol() const { //
    return this->protocol;
}

void NordVpnSettings::setProtocol(Protocol p) {
    if (this->technology == Technology::NordLynx)
        return;
    this->protocol = p;
}

Technology NordVpnSettings::getTechnology() const { //
    return this->technology;
}

void NordVpnSettings::setTechnology(Technology t) {
    this->technology = t;
    if (t == Technology::NordLynx) {
        this->protocol.clear();
        this->obfuscated.clear();
    } else {
        this->protocol = Protocol::UDP;
        this->obfuscated = false;
    }
}

std::string NordVpnSettings::toJSON() {
    json j;
    j["autoconnect"] = this->autoconnect;
    j["cybersec"] = this->cybersec;
    j["dns"] = this->dns;
    for (auto address : this->dnsAddresses)
        j["dnsAddresses"].push_back(address);
    j["killswitch"] = this->killswitch;
    j["notify"] = this->notify;
    if (this->obfuscated.isNotNull())
        j["obfuscated"] = this->obfuscated.value();
    else
        j["obfuscated"] = nullptr;
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
        int n = s.dnsAddresses.size();
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
