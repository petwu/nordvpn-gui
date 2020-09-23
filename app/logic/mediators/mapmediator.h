#ifndef MAPMEDIATOR_H
#define MAPMEDIATOR_H

#include <QObject>

#include "logic/nordvpn/servercontroller.h"

class MapMediator : public QObject {
    Q_OBJECT
  public:
    MapMediator();

  public slots:
    json getAllCountries() { return this->_serverController.getAllCountries(); }

  private:
    ServerController _serverController;
};

#endif // MAPMEDIATOR_H
