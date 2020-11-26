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

/**
 * @brief The QmlDataConverter class provides functions for converting C++
 * classes into QVariants and vice versa which is required to exchange data with
 * the UI.
 *
 * @details The UI uses QML/JavaScript and only some basic C++/Qt types can be
 * converted automatically into JavaScript data types and vice versa. All
 * complex object must be converted manually. See
 * https://doc.qt.io/qt-5/qtqml-cppintegration-data.html for more information.
 *
 * Qt provides two main types that are useful for type conversion:
 * - QVariantMap: The equivalent of a JavaScript object. C++ object attributes
 * are used as map keys. Defined as `QMap<QString, QVariant>`.
 * - QVariantList: The equivalent of a JavaScript array. Defined as
 * `QList<QVariant>`.
 */
class QmlDataConverter {
  public:
    /**
     * @brief Convert any arbitrary JSON object to the equavalent QVariant
     * object.
     */
    static QVariant jsonToQml(const json &j);

    /**
     * @brief Convert a Country to a QML object.
     */
    static QVariantMap countryToQml(const Country &country);

    /**
     * @brief Convert a Location to a QML object.
     */
    static QVariantMap locationToQml(const Location &location);

    /**
     * @brief Convert a Server to a QML object.
     */
    static QVariantMap serverToQml(const Server &server);

    /**
     * @brief Convert NordVpnSettings to a QML object.
     */
    static QVariantMap nordvpnSettingsToQml(const NordVpnSettings &settings);

    /**
     * @brief Convert a WhitelistPortEntry to a QML object.
     */
    static QVariantMap
    whitelistPortsEntryToQml(const WhitelistPortEntry &entry);
};

#endif // JSONCONVERTER_H
