#include "nordvpnsettings.h"

bool WhitelistPortEntry::operator==(const WhitelistPortEntry &other) const {
    return this->portFrom == other.portFrom && this->portTo == other.portTo &&
           this->protocolFlag == other.protocolFlag;
}

bool WhitelistPortEntry::operator<(const WhitelistPortEntry &other) const {
    return this->portFrom < other.portFrom || this->portTo < other.portTo;
}

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

std::vector<std::string> NordVpnSettings::getWhitelistSubnets() const {
    return this->whitelistSubnets;
}

void NordVpnSettings::addSubnetToWhitelist(std::string subnet) {
    this->whitelistSubnets.push_back(subnet);
}

void NordVpnSettings::updateWhitelistSubnet(int index, std::string subnet) {
    if (index < 0 || index >= this->whitelistSubnets.size())
        return;
    this->whitelistSubnets[index] = subnet;
}

void NordVpnSettings::removeSubnetFromWhitelist(int index) {
    if (index < 0 || index >= this->whitelistSubnets.size())
        return;
    this->whitelistSubnets.erase(this->whitelistSubnets.begin() + index);
}

std::vector<WhitelistPortEntry> NordVpnSettings::getWhitelistPorts() const {
    return this->whitelistPorts;
}

void NordVpnSettings::addPortsToWhitelist(WhitelistPortEntry p) {
    this->whitelistPorts.push_back(p);
}

void NordVpnSettings::updatePortsOfWhitelist(int index, WhitelistPortEntry p) {
    if (index < 0 || index >= this->whitelistPorts.size())
        return;
    this->whitelistPorts[index] = p;
}

void NordVpnSettings::removePortsFromWhitelist(int index) {
    if (index < 0 || index >= this->whitelistPorts.size())
        return;
    this->whitelistPorts.erase(this->whitelistPorts.begin() + index);
}
