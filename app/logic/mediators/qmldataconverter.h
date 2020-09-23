#ifndef JSONCONVERTER_H
#define JSONCONVERTER_H

/*
 * array => QVariantList
 * object => QVariantMap
 */

#include <memory>

#include <QVariant>
#include <QVariantList>
#include <QVariantMap>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class QmlDataConverter {
  public:
    static QVariant jsonToQml(json &j);
};

#endif // JSONCONVERTER_H
