#include "mapmediator.h"

MapMediator::MapMediator() {}

QVariant MapMediator::getAllCountries() {
    json::array_t data = this->_serverController.getAllCountries();
    return QmlDataConverter::jsonToQml(data);
}
