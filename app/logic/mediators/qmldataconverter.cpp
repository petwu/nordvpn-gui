#include "qmldataconverter.h"

QVariant QmlDataConverter::jsonToQml(const json &j) {
    QVariant qObj;
    // check JSON type and convert to appropriate Qt/QVariant type
    // (recursively for arrays and objects)
    if (j.is_null()) {
        qObj = QVariant::fromValue(nullptr);
    } else if (j.is_string()) {
        qObj = QString(json::string_t(j).c_str());
    } else if (j.is_boolean()) {
        qObj = json::boolean_t(j);
    } else if (j.is_number_integer()) {
        qObj = QVariant((long long int)(json::number_integer_t(j)));
    } else if (j.is_number_float()) {
        qObj = QVariant(json::number_float_t(j));
    } else if (j.is_array()) {
        QVariantList array;
        for (auto i = j.begin(); i != j.end(); ++i) {
            array << jsonToQml(i.value());
        }
        qObj = array;
    } else if (j.is_object()) {
        QVariantMap object;
        for (auto i = j.begin(); i != j.end(); ++i) {
            QString k(json::string_t(i.key()).c_str());
            QVariant v = jsonToQml(i.value());
            object.insert(k, v);
        }
        qObj = object;
    } else {
        // default constructor of QVariant is equivalent to 'undefined' in
        // JavaScript
    }
    return std::move(qObj);
}

QVariantMap QmlDataConverter::countryToQml(const Country &country) {
    QVariantMap qObj = locationToQml(country);
    qObj["countryCode"] = QString(country.countryCode.c_str());
    qObj["offsetLeft"] = country.offsetLeft;
    qObj["offsetTop"] = country.offsetTop;
    QVariantList cities;
    for (auto city : country.cities) {
        cities << locationToQml(city);
    }
    qObj["cities"] = cities;
    return std::move(qObj);
}

QVariantMap QmlDataConverter::locationToQml(const Location &location) {
    QVariantMap qObj;
    qObj["id"] = location.id;
    qObj["name"] = QString(location.name.c_str());
    qObj["lat"] = location.lat;
    qObj["lng"] = location.lng;
    return std::move(qObj);
}

QVariantMap QmlDataConverter::serverToQml(const Server &server) {
    QVariantMap qObj;
    qObj["id"] = server.id;
    qObj["name"] = QString(server.name.c_str());
    qObj["connectName"] = QString(server.connectName.c_str());
    qObj["hostname"] = QString(server.hostname.c_str());
    qObj["load"] = server.load;
    qObj["countryId"] = server.countryId;
    qObj["cityId"] = server.cityId;
    QVariantList groups;
    for (auto g : server.groups) {
        groups << QString(group2string(g).c_str());
    }
    qObj["groups"] = groups;
    return std::move(qObj);
}

QVariantMap
QmlDataConverter::nordvpnSettingsToQml(const NordVpnSettings &settings) {
    QVariantMap qObj;
    qObj["autoconnect"] = settings.getAutoconnect();
    qObj["cybersec"] = settings.getCybersec();
    qObj["dns"] = settings.getDns();
    QVariantList dnsAddrs;
    for (auto addr : settings.getDnsAddresses())
        dnsAddrs << QString(addr.c_str());
    qObj["dnsAddresses"] = dnsAddrs;
    qObj["maxNumberOfDnsAddresses"] = settings.getMaxNumberOfDnsAddresses();
    qObj["killswitch"] = settings.getKillswitch();
    qObj["notify"] = settings.getNotify();
    qObj["obfuscated"] =
        (settings.getObfuscated().isNull() ? QVariant::fromValue(nullptr)
                                           : settings.getObfuscated().value());
    auto p = protocolToString(settings.getProtocol());
    qObj["protocol"] =
        (p == "null" ? QVariant::fromValue(nullptr) : QString(p.c_str()));
    qObj["technology"] =
        QString(technologyToString(settings.getTechnology()).c_str());
    QVariantList whitelistSubnets;
    for (auto subnet : settings.getWhitelistSubnets())
        whitelistSubnets << QString(subnet.c_str());
    qObj["whitelistSubnets"] = whitelistSubnets;
    QVariantList whitelistPorts;
    for (auto ports : settings.getWhitelistPorts())
        whitelistPorts << whitelistPortsEntryToQml(ports);
    qObj["whitelistPorts"] = whitelistPorts;
    return std::move(qObj);
}

QVariantMap
QmlDataConverter::whitelistPortsEntryToQml(const WhitelistPortEntry &entry) {
    QVariantMap qObj;
    qObj["portFrom"] = entry.portFrom;
    qObj["portTo"] = entry.portTo;
    qObj["protocolFlag"] = static_cast<int>(entry.protocolFlag);
    return std::move(qObj);
}
