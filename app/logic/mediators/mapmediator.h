#ifndef MAPMEDIATOR_H
#define MAPMEDIATOR_H

#include <QObject>
#include <QVariant>

#include "logic/nordvpn/servercontroller.h"
#include "qmldataconverter.h"

class MapMediator : public QObject {
    Q_OBJECT
  public:
    MapMediator();

  public slots:
    QVariant getAllCountries();

  private:
    ServerController _serverController;
};

#endif // MAPMEDIATOR_H
