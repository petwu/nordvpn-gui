#ifndef MAPMEDIATOR_H
#define MAPMEDIATOR_H

#include <QObject>
#include <QVariant>

#include "logic/nordvpn/servercontroller.h"
#include "logic/nordvpn/statuscontroller.h"
#include "qmldataconverter.h"

class MapMediator : public QObject, public IConnectionInfoSubscription {

    Q_OBJECT

    Q_PROPERTY(qint32 connectingCountryId READ _getConnectingCountryId NOTIFY
                   connectingIdChanged)
    Q_PROPERTY(qint32 connectedCountryId READ _getConnectedCountryId NOTIFY
                   connectedIdChanged)

  public:
    MapMediator();

  public slots:
    QVariant getAllCountries();

  signals:
    void connectingIdChanged(qint32 countryId);
    void connectedIdChanged(qint32 countryId);

  private:
    ServerController _serverController;
    StatusController _statusController;
    json::array_t _countries;
    int32_t _connectingCountryId = 0;
    int32_t _connectedCountryId = 0;
    qint32 _getConnectingCountryId();
    qint32 _getConnectedCountryId();
    int32_t _getCountryId(std::string name);
    void update(const ConnectionInfo &newStatus);
};

#endif // MAPMEDIATOR_H
