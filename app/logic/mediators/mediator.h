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
                   connectingCountryIdChanged)
    Q_PROPERTY(qint32 connectedCountryId READ _getConnectedCountryId NOTIFY
                   connectedCountryIdChanged)
    Q_PROPERTY(qint32 connectedCityId READ _getConnectedCityId NOTIFY
                   connectedCityIdChanged)
    Q_PROPERTY(qint32 connectedServerNr READ _getConnectedServerNr NOTIFY
                   connectedServerNrChanged)
    Q_PROPERTY(
        QString connectedIP READ _getConnectedIP NOTIFY connectedIPChanged)
    Q_PROPERTY(
        qint64 receivedBytes READ _getReceivedBytes NOTIFY receivedBytesChanged)
    Q_PROPERTY(qint64 sentBytes READ _getSentBytes NOTIFY sentBytesChanged)
    Q_PROPERTY(
        QVariant countryList READ _getCountryList NOTIFY countryListChanged)
    Q_PROPERTY(QVariantList recentCountries READ _getRecentCountries NOTIFY
                   recentCountriesChanged)
    Q_PROPERTY(QVariantList connectedServerGroups READ _getConnectedServerGroups
                   NOTIFY connectedServerGroupsChanged)
    Q_PROPERTY(
        QVariantList connectingServerGroups READ _getConnectingServerGroups
            NOTIFY connectingServerGroupsChanged)

  public slots:
    void quickConnect();
    void connectToCountryById(quint32 id);
    void connectToCityById(quint32 id);
    void connectToServerById(quint32 serverId);
    void connectToSpecialtyGroup(quint32 groupId);
    void connectToCountryByIdAndGroup(quint32 id, quint32 groupId);
    void cancelConnection();
    void disconnect();
    void removeFromRecentsList(quint32 countryId);
    QVariantList getServers(qint32 countryId, qint32 cityId);
    QVariantList getSpecialtyCountries(quint32 groupId);
    QVariantList getSpecialtyServers(quint32 groupId, qint32 countryId);
    void rate(quint8 rating);

  signals:
    void areConnectionCommandsPausedChanged(bool);
    void disconnectedChanged(bool);
    void connectingChanged(bool);
    void connectedChanged(bool);
    void isRatingPossibleChanged(bool);
    void connectingCountryIdChanged(qint32);
    void connectedCountryIdChanged(qint32);
    void connectedCityIdChanged(qint32);
    void connectedServerNrChanged(qint32);
    void connectedIPChanged(QString);
    void receivedBytesChanged(qint64);
    void sentBytesChanged(qint64);
    void countryListChanged(QVariant);
    void recentCountriesChanged(QVariantList);
    void connectedServerGroupsChanged(QVariantList);
    void connectingServerGroupsChanged(QVariantList);

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
    int32_t _connectedCityId = -1;
    qint32 _getConnectedCityId();
    void _setConnectedCityId(qint32 value);
    int32_t _connectedServerNr = 0;
    qint32 _getConnectedServerNr();
    void _setConnectedServerNr(qint32 value);
    std::string _connectedIP = "";
    QString _getConnectedIP();
    void _setConnectedIP(std::string value);
    uint64_t _receivedBytes = 0;
    qint64 _getReceivedBytes();
    void _setReceivedBytes(uint64_t value);
    uint64_t _sentBytes = 0;
    qint64 _getSentBytes();
    void _setSentBytes(uint64_t value);
    QVariant _getCountryList();
    std::vector<Country> _recentCountries;
    QVariantList _getRecentCountries();
    std::vector<Group> _connectedServerGroups;
    QVariantList _getConnectedServerGroups();
    void _setConnectedServerGroups(std::vector<Group> groups);
    std::vector<Group> _connectingServerGroups;
    QVariantList _getConnectingServerGroups();
    void _setConnectingServerGroups(std::vector<Group> groups);

    void updateConnectionInfo(const ConnectionInfo &newStatus) override;
    void updateRecents(const std::vector<Country> &newRecents) override;
    void updateCountryList(const std::vector<Country> &countryList) override;
};

#endif // MEDIATOR_H
