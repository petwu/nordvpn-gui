#include "qmldataconverter.h"

QVariant QmlDataConverter::jsonToQml(const json &j) {
    QVariant qVar;
    // check JSON type and convert to appropriate Qt/QVariant type
    // (recursively for arrays and objects)
    if (j.is_null()) {
        qVar = QVariant::fromValue(nullptr);
    } else if (j.is_string()) {
        qVar = QString(json::string_t(j).c_str());
    } else if (j.is_boolean()) {
        qVar = json::boolean_t(j);
    } else if (j.is_number_integer()) {
        qVar = QVariant((long long int)(json::number_integer_t(j)));
    } else if (j.is_number_float()) {
        qVar = QVariant(json::number_float_t(j));
    } else if (j.is_array()) {
        QVariantList array;
        for (auto i = j.begin(); i != j.end(); ++i) {
            array << jsonToQml(i.value());
        }
        qVar = array;
    } else if (j.is_object()) {
        QVariantMap object;
        for (auto i = j.begin(); i != j.end(); ++i) {
            QString k(json::string_t(i.key()).c_str());
            QVariant v = jsonToQml(i.value());
            object.insert(k, v);
        }
        qVar = object;
    } else {
        // default constructor of QVariant is equivalent to 'undefined' in
        // JavaScript
    }
    return std::move(qVar);
}
