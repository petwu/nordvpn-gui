#ifndef JSONCONVERTER_H
#define JSONCONVERTER_H

#include <QVariant>
#include <QVariantMap>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include "data/models/country.h"
#include "data/models/location.h"
#include "data/models/server.h"
#include "logic/models/envinfo.h"
#include "logic/models/nordvpnsettings.h"
#include "logic/models/whitelistportentry.h"

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
    static auto jsonToQml(const json &j) -> QVariant;

    /**
     * @brief Convert a Country to a QML object.
     */
    static auto countryToQml(const Country &country) -> QVariantMap;

    /**
     * @brief Convert a Location to a QML object.
     */
    static auto locationToQml(const Location &location) -> QVariantMap;

    /**
     * @brief Convert a Server to a QML object.
     */
    static auto serverToQml(const Server &server) -> QVariantMap;

    /**
     * @brief Convert NordVpnSettings to a QML object.
     */
    static auto nordvpnSettingsToQml(const NordVpnSettings &settings)
        -> QVariantMap;

    /**
     * @brief Convert a WhitelistPortEntry to a QML object.
     */
    static auto whitelistPortsEntryToQml(const WhitelistPortEntry &entry)
        -> QVariantMap;

    /**
     * @brief Convert a EnvInfo object to a QML object.
     */
    static auto envInfoToQml(const EnvInfo &envInfo) -> QVariantMap;
};

#endif // JSONCONVERTER_H