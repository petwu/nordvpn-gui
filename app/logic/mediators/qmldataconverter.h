#ifndef JSONCONVERTER_H
#define JSONCONVERTER_H

#include <iostream>
#include <memory>

#include <QVariant>
#include <QVariantList>
#include <QVariantMap>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class QmlDataConverter {
  public:
    static QVariant jsonToQml(const json &j);
};

#endif // JSONCONVERTER_H
