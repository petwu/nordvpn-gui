#include "nordvpnsettings.h"

#include <algorithm>
#include <ext/alloc_traits.h>
#include <utility>

auto NordVpnSettings::getAutoconnect() const -> bool { //
    return this->autoconnect;
}

void NordVpnSettings::setAutoconnect(bool enabled) { //
    this->autoconnect = enabled;
}

auto NordVpnSettings::getCybersec() const -> bool { //
    return this->cybersec;
}

void NordVpnSettings::setCybersec(bool enabled) { //
    this->cybersec = enabled;
    if (enabled) {
        this->dns = false;
    }
}

auto NordVpnSettings::getDns() const -> bool { //
    return this->dns;
}

void NordVpnSettings::setDns(bool enabled) {
    this->dns = enabled;
    if (enabled) {
        this->cybersec = false;
    }
}

auto NordVpnSettings::getDnsAddresses() const -> std::vector<std::string> {
    return this->dnsAddresses;
}

auto NordVpnSettings::getMaxNumberOfDnsAddresses() const -> int {
    return this->dnsAddresses.size();
}

void NordVpnSettings::setDnsAddress(int nth, const std::string &addr) {
    if (nth >= this->dnsAddresses.size() || addr.empty()) {
        return;
    }
    this->dnsAddresses[nth] = addr;
}

auto NordVpnSettings::getKillswitch() const -> bool { //
    return this->killswitch;
}

void NordVpnSettings::setKillswitch(bool enabled) { //
    this->killswitch = enabled;
}

auto NordVpnSettings::getNotify() const -> bool { //
    return this->notify;
}

void NordVpnSettings::setNotify(bool enabled) { //
    this->notify = enabled;
}

auto NordVpnSettings::getObfuscated() const -> Nullable<bool> { //
    return this->obfuscated;
}

void NordVpnSettings::setObfuscated(bool enabled) {
    this->obfuscated = enabled;
    if (enabled) {
        this->dns = false;
    }
}

auto NordVpnSettings::getProtocol() const -> Nullable<Protocol> { //
    return this->protocol;
}

void NordVpnSettings::setProtocol(Protocol p) {
    if (this->technology == Technology::NordLynx) {
        return;
    }
    this->protocol = p;
}

auto NordVpnSettings::getTechnology() const -> Technology { //
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

auto NordVpnSettings::getWhitelistSubnets() const -> std::vector<std::string> {
    return this->whitelistSubnets;
}

void NordVpnSettings::addSubnetToWhitelist(const std::string &subnet) {
    this->whitelistSubnets.push_back(subnet);
}

void NordVpnSettings::updateWhitelistSubnet(int index, std::string subnet) {
    if (index < 0 || index >= this->whitelistSubnets.size()) {
        return;
    }
    this->whitelistSubnets[index] = std::move(subnet);
}

void NordVpnSettings::removeSubnetFromWhitelist(int index) {
    if (index < 0 || index >= this->whitelistSubnets.size()) {
        return;
    }
    this->whitelistSubnets.erase(this->whitelistSubnets.begin() + index);
}

auto NordVpnSettings::getWhitelistPorts() const
    -> std::vector<WhitelistPortEntry> {
    return this->whitelistPorts;
}

void NordVpnSettings::addPortsToWhitelist(WhitelistPortEntry p) {
    this->whitelistPorts.push_back(p);
}

void NordVpnSettings::updatePortsOfWhitelist(int index, WhitelistPortEntry p) {
    if (index < 0 || index >= this->whitelistPorts.size()) {
        return;
    }
    this->whitelistPorts[index] = p;
}

void NordVpnSettings::removePortsFromWhitelist(int index) {
    if (index < 0 || index >= this->whitelistPorts.size()) {
        return;
    }
    this->whitelistPorts.erase(this->whitelistPorts.begin() + index);
}
