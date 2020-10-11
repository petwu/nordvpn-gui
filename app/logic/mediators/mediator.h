#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <QObject>
#include <QVariant>

#include "logic/nordvpn/servercontroller.h"
#include "logic/nordvpn/statuscontroller.h"
#include "qmldataconverter.h"

class Mediator : public QObject,
                 public IConnectionInfoSubscription,
                 public ICountriesSubscription {
    Q_OBJECT

  public:
    Mediator();

    Q_PROPERTY(
        bool areConnectionCommandsPaused READ _getAreConnectionCommandsPaused
            NOTIFY areConnectionCommandsPausedChanged)
    Q_PROPERTY(
        bool isDisconnected READ _getIsDisconnected NOTIFY disconnectedChanged)
    Q_PROPERTY(bool isConnecting READ _getIsConnecting NOTIFY connectingChanged)
    Q_PROPERTY(bool isConnected READ _getIsConnected NOTIFY connectedChanged)
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
    Q_PROPERTY(QVariantList recentCountries READ _getRecentCountries NOTIFY
                   recentCountriesChanged)

  public slots:
    void quickConnect();
    void connectToCountryById(quint32 id);
    void connectToServerById(quint32 serverId);
    void disconnect();
    void removeFromRecentsList(quint32 countryId);
    QVariantList getServers(qint32 countryId, qint32 cityId);
    void rate(quint8 rating);

  signals:
    void areConnectionCommandsPausedChanged(bool);
    void disconnectedChanged(bool);
    void connectingChanged(bool);
    void connectedChanged(bool);
    void isRatingPossibleChanged(bool);
    void connectingIdChanged(qint32);
    void connectedIdChanged(qint32);
    void connectedServerIdChanged(qint32);
    void connectedIPChanged(QString);
    void countryListChanged(QVariant);
    void recentCountriesChanged(QVariantList);

  private:
    ServerController &_serverController = ServerController::getInstance();
    StatusController &_statusController = StatusController::getInstance();
    std::vector<Country> _countries;
    std::vector<Server> _servers;

    bool _areConnectionCommandsPaused = false;
    bool _getAreConnectionCommandsPaused();
    void _setAreConnectionCommandsPaused(bool value);
    bool _isDisconnected = false;
    bool _getIsDisconnected();
    void _setIsDisconnected(bool value);
    bool _isConnecting = false;
    bool _getIsConnecting();
    void _setIsConnecting(bool value);
    bool _isConnected = false;
    bool _getIsConnected();
    void _setIsConnected(bool value);
    bool _isRatingPossbile = false;
    bool _getIsRatingPossible();
    void _setIsRatingPossible(bool value);
    int32_t _connectingCountryId = -1;
    qint32 _getConnectingCountryId();
    void _setConnectingCountryId(qint32 value);
    int32_t _connectedCountryId = -1;
    qint32 _getConnectedCountryId();
    void _setConnectedCountryId(qint32 value);
    int32_t _connectedServerId = 0;
    qint32 _getConnectedServerId();
    void _setConnectedServerId(qint32 value);
    std::string _connectedIP = "";
    QString _getConnectedIP();
    void _setConnectedIP(std::string value);
    QVariant _getCountryList();
    std::vector<Country> _recentCountries;
    QVariantList _getRecentCountries();

    void updateConnectionInfo(const ConnectionInfo &newStatus) override;
    void updateRecents(const std::vector<Country> &newRecents) override;
    void updateCountryList(const std::vector<Country> &countryList) override;
};

#endif // MEDIATOR_H
