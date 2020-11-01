#ifndef JSONCONVERTER_H
#define JSONCONVERTER_H

#include <iostream>
#include <memory>
#include <vector>

#include <QVariant>
#include <QVariantList>
#include <QVariantMap>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "data/repositories/serverrepository.h"
#include "logic/models/nordvpnsettings.h"

class QmlDataConverter {
  public:
    static QVariant jsonToQml(const json &j);
    static QVariantMap countryToQml(const Country &country);
    static QVariantMap locationToQml(const Location &location);
    static QVariantMap serverToQml(const Server &server);
    static QVariantMap nordvpnSettingsToQml(const NordVpnSettings &settings);
    static QVariantMap
    whitelistPortsEntryToQml(const WhitelistPortEntry &entry);
};

#endif // JSONCONVERTER_H
