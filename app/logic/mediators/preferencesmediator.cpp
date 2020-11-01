#include "preferencesmediator.h"

PreferencesMediator::PreferencesMediator() {
    this->_nordvpnSettings = this->_preferencesController.getNordvpnSettings();
}

QVariantMap PreferencesMediator::_getNordvpnSettings() {
    return QmlDataConverter::nordvpnSettingsToQml(this->_nordvpnSettings);
}

void PreferencesMediator::_nordvpnSettingsChanged() {
    this->nordvpnSettingsChanged(
        QmlDataConverter::nordvpnSettingsToQml(this->_nordvpnSettings));
}

void PreferencesMediator::refreshSettings() {
    this->_nordvpnSettings = this->_preferencesController.getNordvpnSettings();
    this->_nordvpnSettingsChanged();
}

void PreferencesMediator::setAutoconnect(bool enabled) {
    if (this->_nordvpnSettings.getAutoconnect() == enabled)
        return;
    this->_nordvpnSettings.setAutoconnect(enabled);
    this->_nordvpnSettingsChanged();
}

void PreferencesMediator::setCybersec(bool enabled) {
    if (this->_nordvpnSettings.getCybersec() == enabled)
        return;
    this->_nordvpnSettings.setCybersec(enabled);
    this->_nordvpnSettingsChanged();
}

void PreferencesMediator::setDns(bool enabled) {
    if (this->_nordvpnSettings.getDns() == enabled)
        return;
    this->_nordvpnSettings.setDns(enabled);
    this->_nordvpnSettingsChanged();
}

void PreferencesMediator::setDnsAddress(int i, QString addr) {
    if (this->_nordvpnSettings.getDnsAddresses()[i] == addr.toStdString())
        return;
    this->_nordvpnSettings.setDnsAddress(i, addr.toStdString());
    this->_nordvpnSettingsChanged();
}

void PreferencesMediator::setKillswitch(bool enabled) {
    if (this->_nordvpnSettings.getKillswitch() == enabled)
        return;
    this->_nordvpnSettings.setKillswitch(enabled);
    this->_nordvpnSettingsChanged();
}

void PreferencesMediator::setNotify(bool enabled) {
    if (this->_nordvpnSettings.getNotify() == enabled)
        return;
    this->_nordvpnSettings.setNotify(enabled);
    this->_nordvpnSettingsChanged();
}

void PreferencesMediator::setObfuscated(bool enabled) {
    if (this->_nordvpnSettings.getObfuscated() == enabled)
        return;
    this->_nordvpnSettings.setObfuscated(enabled);
    this->_nordvpnSettingsChanged();
}

void PreferencesMediator::setProtocol(QString protocol) {
    auto p = protocolFromString(protocol.toStdString());
    if (this->_nordvpnSettings.getProtocol() == p)
        return;
    this->_nordvpnSettings.setProtocol(p);
    this->_nordvpnSettingsChanged();
}

void PreferencesMediator::setTechnology(QString technology) {
    auto t = technologyFromString(technology.toStdString());
    if (this->_nordvpnSettings.getTechnology() == t)
        return;
    this->_nordvpnSettings.setTechnology(t);
    this->_nordvpnSettingsChanged();
}

void PreferencesMediator::addSubnetToWhitelist() {
    this->_nordvpnSettings.addSubnetToWhitelist("");
    this->_nordvpnSettingsChanged();
}

void PreferencesMediator::updateWhitelistSubnet(int i, QString subnet) {
    if (i < 0)
        return;
    if (i >= this->_nordvpnSettings.getWhitelistSubnets().size())
        this->_nordvpnSettings.addSubnetToWhitelist(subnet.toStdString());
    else
        this->_nordvpnSettings.updateWhitelistSubnet(i, subnet.toStdString());
}

void PreferencesMediator::removeSubnetFromWhitelist(int index) {
    this->_nordvpnSettings.removeSubnetFromWhitelist(index);
    this->_nordvpnSettingsChanged();
}

void PreferencesMediator::addPortsToWhitelist() {
    this->_nordvpnSettings.addPortsToWhitelist(WhitelistPortEntry{});
    this->_nordvpnSettingsChanged();
}

#include <QDebug>
void PreferencesMediator::updateWhitelistPorts(int index, QString portFrom,
                                               QString portTo,
                                               uint protocolFlag) {
    if (index < 0)
        return;
    WhitelistPortEntry entry;
    bool cnvtSuccess;
    uint from = portFrom.toUInt(&cnvtSuccess);
    entry.portFrom = cnvtSuccess ? from : -1;
    uint to = portTo.toInt(&cnvtSuccess);
    entry.portTo = cnvtSuccess ? to : -1;
    entry.protocolFlag = static_cast<Protocol>(protocolFlag);
    if (index < this->_nordvpnSettings.getWhitelistPorts().size())
        this->_nordvpnSettings.updatePortsOfWhitelist(index, entry);
    else
        this->_nordvpnSettings.addPortsToWhitelist(entry);
}

void PreferencesMediator::removePortsFromWhitelist(int index) {
    this->_nordvpnSettings.removePortsFromWhitelist(index);
    this->_nordvpnSettingsChanged();
}

void PreferencesMediator::saveNordvpnSettings() {
    // clear invalid dns addresses
    auto addrs = this->_nordvpnSettings.getDnsAddresses();
    for (size_t i = 0; i < addrs.size(); i++)
        if (!this->isValidIpAddress(QString(addrs[i].c_str())))
            this->_nordvpnSettings.setDnsAddress(i, "");

    // remove invalid entries from whitelist
    std::vector<int> invalidSubnets;
    auto subnets = this->_nordvpnSettings.getWhitelistSubnets();
    for (size_t i = 0; i < subnets.size(); i++)
        if (!this->isValidSubnetMask(QString(subnets[i].c_str())))
            invalidSubnets.push_back(i);
    std::sort(invalidSubnets.begin(), invalidSubnets.end(), std::greater<>());
    for (auto index : invalidSubnets)
        this->_nordvpnSettings.removeSubnetFromWhitelist(index);
    std::vector<int> invalidPorts;
    auto ports = this->_nordvpnSettings.getWhitelistPorts();
    for (size_t i = 0; i < ports.size(); i++)
        if (ports[i].portFrom > 65535 || ports[i].portTo > 65535 ||
            ports[i].portTo < ports[i].portFrom)
            invalidPorts.push_back(i);
    for (auto index : invalidPorts)
        this->_nordvpnSettings.removePortsFromWhitelist(index);

    // save the valid settings
    this->_preferencesController.updateNordvpnSettings(this->_nordvpnSettings);
}

void PreferencesMediator::restoreDefaultNordvpnSettings() {
    this->_preferencesController.restoreDefaultNordvpnSettings();
    this->nordvpnSettingsChanged(this->_getNordvpnSettings());
}

// clang-format off

/**
 * @brief IPv4_REGEX is a regular expression to validate IPv4 adresses.
 *
 * Format: [1;255].[1;255].[1;255].[1;255]
 */
const std::string IPv4_REGEX =
    "^"
    "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?\\.){3}" // [1;255].[1;255].[1;255].
    "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)"       // [1;255]
    "$";

/**
 * @brief IPv6_REGEX is a regular expression to validate IPv6 adresses.
 *
 * Format:
 *
 * (
 * ([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|          # 1:2:3:4:5:6:7:8
 * ([0-9a-fA-F]{1,4}:){1,7}:|                         # 1::                              1:2:3:4:5:6:7::
 * ([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|         # 1::8             1:2:3:4:5:6::8  1:2:3:4:5:6::8
 * ([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|  # 1::7:8           1:2:3:4:5::7:8  1:2:3:4:5::8
 * ([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|  # 1::6:7:8         1:2:3:4::6:7:8  1:2:3:4::8
 * ([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|  # 1::5:6:7:8       1:2:3::5:6:7:8  1:2:3::8
 * ([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|  # 1::4:5:6:7:8     1:2::4:5:6:7:8  1:2::8
 * [0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|       # 1::3:4:5:6:7:8   1::3:4:5:6:7:8  1::8
 * :((:[0-9a-fA-F]{1,4}){1,7}|:)|                     # ::2:3:4:5:6:7:8  ::2:3:4:5:6:7:8 ::8       ::
 * fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}|     # fe80::7:8%eth0   fe80::7:8%1     (link-local IPv6 addresses with zone index)
 * ::(ffff(:0{1,4}){0,1}:){0,1}
 * ((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\.){3,3}
 * (25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|          # ::255.255.255.255   ::ffff:255.255.255.255  ::ffff:0:255.255.255.255  (IPv4-mapped IPv6 addresses and IPv4-translated addresses)
 * ([0-9a-fA-F]{1,4}:){1,4}:
 * ((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\.){3,3}
 * (25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])           # 2001:db8:3:4::192.0.2.33  64:ff9b::192.0.2.33 (IPv4-Embedded IPv6 Address)
 * )
 *
 * Source:
 * https://stackoverflow.com/questions/53497/regular-expression-that-matches-valid-ipv6-addresses
 */
const std::string IPv6_REGEX =
        "^("
        "([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|"
        "([0-9a-fA-F]{1,4}:){1,7}:|"
        "([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|"
        "([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|"
        "([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|"
        "([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|"
        "([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|"
        "[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|"
        ":((:[0-9a-fA-F]{1,4}){1,7}|:)|"
        "fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}|"
        "::(ffff(:0{1,4}){0,1}:){0,1}"
        "((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.){3,3}"
        "(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|"
        "([0-9a-fA-F]{1,4}:){1,4}:"
        "((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.){3,3}"
        "(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])"
        ")$";

/**
 * @brief IPv4_SUBNET_REGEX is a regular expression to validate IPv4 subnet masks.
 *
 * Format: <valid IPv4 address>/[0;32]
 */
const std::string IPv4_SUBNET_REGEX =
    IPv4_REGEX.substr(0, IPv4_REGEX.size()-1) + "/(3[0-2]|[12]?[0-9])$";

/**
 * @brief IPv6_SUBNET_REGEX is a regular expression to validate IPv6 subnet masks.
 *
 * Format: <valid IPv6 address>/[0;128]
 */
const std::string IPv6_SUBNET_REGEX =
    IPv6_REGEX.substr(0, IPv6_REGEX.size()-1) + "/(12[0-8]|1[01][0-9]|[1-9]?[0-9])$";

// clang-format on

bool PreferencesMediator::isValidIpAddress(QString ip) {
    if (std::regex_match(ip.toStdString(), std::regex(IPv4_REGEX)))
        return true;
    if (std::regex_match(ip.toStdString(), std::regex(IPv6_REGEX)))
        return true;
    return false;
}

bool PreferencesMediator::isValidSubnetMask(QString subnet) {
    if (std::regex_match(subnet.toStdString(), std::regex(IPv4_SUBNET_REGEX)))
        return true;
    if (std::regex_match(subnet.toStdString(), std::regex(IPv6_SUBNET_REGEX)))
        return true;
    return false;
}

bool PreferencesMediator::isValidPort(QString port) {
    if (port == "")
        return false;
    bool canConvert;
    uint p = port.toUInt(&canConvert);
    return canConvert && p >= 0 && p <= 65535;
}