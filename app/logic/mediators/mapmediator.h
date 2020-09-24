#ifndef MAPMEDIATOR_H
#define MAPMEDIATOR_H

#include <QObject>
#include <QVariant>

#include "logic/nordvpn/servercontroller.h"
#include "logic/nordvpn/statuscontroller.h"
#include "qmldataconverter.h"

class MapMediator : public QObject, public IConnectionInfoSubscription {

    Q_OBJECT

    Q_PROPERTY(
        bool isDisconnected READ _isDisconnected NOTIFY disconnectedChanged)
    Q_PROPERTY(bool isConnecting READ _isConnecting NOTIFY connectingChanged)
    Q_PROPERTY(bool isConnected READ _isConnected NOTIFY connectedChanged)
    Q_PROPERTY(bool isRatingPossbile READ _getIsRatingPossible NOTIFY
                   isRatingPossibleChanged)
    Q_PROPERTY(qint32 connectingCountryId READ _getConnectingCountryId NOTIFY
                   connectingIdChanged)
    Q_PROPERTY(qint32 connectedCountryId READ _getConnectedCountryId NOTIFY
                   connectedIdChanged)
    Q_PROPERTY(qint32 connectedServerId READ _getConnectedServerId NOTIFY
                   connectedServerIdChanged)
    Q_PROPERTY(
        QString connectedIP READ _getConnectedIP NOTIFY connectedIPChanged)
    Q_PROPERTY(
        QVariant countryList READ _getCountryList NOTIFY countryListChanged)

  public:
    MapMediator();

  public slots:
    void quickConnect();
    void connectToCountryById(quint32 id);
    void disconnect();
    void rate(quint8 rating);

  signals:
    void disconnectedChanged(bool connected);
    void connectingChanged(bool connecting);
    void connectedChanged(bool connected);
    void isRatingPossibleChanged(bool possible);
    void connectingIdChanged(qint32 countryId);
    void connectedIdChanged(qint32 countryId);
    void connectedServerIdChanged(qint32 serverId);
    void connectedIPChanged(QString ip);
    void countryListChanged(QVariant countryList);

  private:
    ServerController _serverController;
    StatusController _statusController;
    json::array_t _countries;

    bool _disconnected = false;
    bool _isDisconnected();
    bool _connecting = false;
    bool _isConnecting();
    bool _connected = false;
    bool _isConnected();
    bool _isRatingPossbile = false;
    bool _getIsRatingPossible();
    int32_t _connectingCountryId = -1;
    qint32 _getConnectingCountryId();
    int32_t _connectedCountryId = -1;
    qint32 _getConnectedCountryId();
    int32_t _connectedServerId = 0;
    qint32 _getConnectedServerId();
    std::string _connectedIP = "";
    QString _getConnectedIP();

    void update(const ConnectionInfo &newStatus);
    QVariant _getCountryList();
};

#endif // MAPMEDIATOR_H
