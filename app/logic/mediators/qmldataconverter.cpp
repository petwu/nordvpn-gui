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
    return std::move(qObj);
}

NordVpnSettings
QmlDataConverter::nordvpnSettingsFromQml(const QVariantMap &qObj) {
    NordVpnSettings settings;
    auto propMap = qObj.toStdMap();

    if (propMap.count("autoconnect") == 1 &&
        propMap["autoconnect"].canConvert<bool>())
        settings.setAutoconnect(propMap["autoconnect"].value<bool>());

    if (propMap.count("cybersec") == 1 &&
        propMap["cybersec"].canConvert<bool>())
        settings.setCybersec(propMap["cybersec"].value<bool>());

    if (propMap.count("dns") == 1 && propMap["dns"].canConvert<bool>())
        settings.setDns(propMap["dns"].value<bool>());

    if (settings.getDns() == true && propMap.count("dnsAddresses") == 1 &&
        propMap["dnsAddresses"].canConvert<QVariantList>()) {
        int i = 0;
        int n = settings.getMaxNumberOfDnsAddresses();
        for (auto addrVar : propMap["dnsAddresses"].value<QVariantList>()) {
            if (i == n)
                break;
            if (addrVar.canConvert<QString>())
                settings.setDnsAddress(i, addrVar.toString().toStdString());
            i++;
        }
    }

    if (propMap.count("killswitch") == 1 &&
        propMap["killswitch"].canConvert<bool>())
        settings.setKillswitch(propMap["killswitch"].value<bool>());

    if (propMap.count("notify") == 1 && propMap["notify"].canConvert<bool>())
        settings.setNotify(propMap["notify"].value<bool>());

    if (propMap.count("obfuscated") == 1 &&
        propMap["obfuscated"].canConvert<bool>())
        settings.setObfuscated(propMap["obfuscated"].value<bool>());

    if (propMap.count("protocol") == 1 &&
        propMap["protocol"].canConvert<QString>() &&
        !propMap["protocol"].canConvert(QMetaType::Nullptr))
        settings.setProtocol(
            protocolFromString(propMap["protocol"].toString().toStdString()));

    if (propMap.count("technology") == 1 &&
        propMap["technology"].canConvert<QString>() &&
        !propMap["technology"].canConvert(QMetaType::Nullptr))
        settings.setTechnology(technologyFromString(
            propMap["technology"].toString().toStdString()));

    return std::move(settings);
}
