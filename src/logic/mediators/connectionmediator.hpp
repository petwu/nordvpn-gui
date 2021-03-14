#ifndef CONNECTIONMEDIATOR_HPP
#define CONNECTIONMEDIATOR_HPP

#include <QObject>
#include <QString>
#include <QVariant>
#include <QVariantList>
#include <QtCore>
#include <cstdint>
#include <string>
#include <vector>

#include "data/enums/group.hpp"
#include "data/models/country.hpp"
#include "logic/models/connectioninfo.hpp"
#include "logic/nordvpn/connectioncontroller.hpp"
#include "logic/nordvpn/countrycontroller.hpp"
#include "logic/nordvpn/servercontroller.hpp"
#include "logic/nordvpn/statuscontroller.hpp"
#include "logic/subscriptions/iconnectioninfosubscription.hpp"
#include "logic/subscriptions/icountriessubscription.hpp"

/**
 * @brief The ConnectionMediator class is a quite important mediator. It is
 * responsible for communicating all things with the UI that are related to the
 * connection.
 *
 * @details The following things are part of this mediator:
 * - Providing lists of available countries (including their cities) and
 * servers.
 * - providing information about the current connection statue like
 *   - connected: yes? no? connecting?
 *   - information about the server (IP, domain, number, country, city)
 *   - amount of sent/received data
 * - functions to control the connection to a server:
 *   - connect randomly
 *   - connect to a specifiy country/city/server group/server
 *   - abort connection establishment
 *   - disconnect
 * - rate the last connection
 */
class ConnectionMediator : public QObject,
                           public IConnectionInfoSubscription,
                           public ICountriesSubscription {
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): Qt is out of scope
    Q_OBJECT

  public:
    ConnectionMediator();

    /**
     * @brief Readony property that tells the UI that all connection-related
     * commands (connect, disconnect, etc.) should be paused.
     * @details This property exists because there is a delay between clicking
     * the "connect"-button, connecting, refreshing the status and then
     * eventually reflect the updated status in the UI. Also the NordVPN CLI
     * takes sometimes a notable duration to update their status (command:
     * "nordvpn status") which has an influence on this application too. This
     * property therefore prevents things like clicking the connect button 5
     * times in a row or doing unwanted things because the UI hasn't changed
     * yet.
     */
    Q_PROPERTY(
        bool areConnectionCommandsPaused READ _getAreConnectionCommandsPaused
            NOTIFY areConnectionCommandsPausedChanged)

    /**
     * @brief Readonly property whether disconnected from the VPN services or
     * not.
     * @details #isDisconnected, #isConnecting and #isConnected are mutually
     * exclusive. Exactly one of them is always true.
     */
    Q_PROPERTY(
        bool isDisconnected READ _getIsDisconnected NOTIFY disconnectedChanged)

    /**
     * @brief Readonly property whether an connection is beeing established.
     * @details #isDisconnected, #isConnecting and #isConnected are mutually
     * exclusive. Exactly one of them is always true.
     */
    Q_PROPERTY(bool isConnecting READ _getIsConnecting NOTIFY connectingChanged)

    /**
     * @brief Readonly property whether a connection to a VPN server is active.
     * @details #isDisconnected, #isConnecting and #isConnected are mutually
     * exclusive. Exactly one of them is always true.
     */
    Q_PROPERTY(bool isConnected READ _getIsConnected NOTIFY connectedChanged)

    /**
     * @brief Readonly property whether rating the last connection is possible.
     * @details This property is only true after a state change from "connected"
     * to "disconnected". The property will reset automatically after some time.
     */
    Q_PROPERTY(bool isRatingPossible READ _getIsRatingPossible NOTIFY
                   isRatingPossibleChanged)

    /**
     * @brief While the connection to a server is being established, this
     * readonly property holds the ID of the country the server resides in. The
     * value will be <0 if #isConnecting is false.
     */
    Q_PROPERTY(qint32 connectingCountryId READ _getConnectingCountryId NOTIFY
                   connectingCountryIdChanged)

    /**
     * @brief Readonly property holding the ID of the country the  currently
     * connected server is located in. The value will be <0 if #isConnected is
     * false.
     */
    Q_PROPERTY(qint32 connectedCountryId READ _getConnectedCountryId NOTIFY
                   connectedCountryIdChanged)

    /**
     * @brief Readonly property holding the ID of the city the currently
     * connected server is located in. The value will be <0 if #isConnected is
     * false.
     */
    Q_PROPERTY(qint32 connectedCityId READ _getConnectedCityId NOTIFY
                   connectedCityIdChanged)

    /**
     * @brief Readonly property holding the decimal number of the currenlty
     * connected server. The value will be 0 if #isConnected is false.
     */
    Q_PROPERTY(qint32 connectedServerNr READ _getConnectedServerNr NOTIFY
                   connectedServerNrChanged)

    /**
     * @brief Readonly property holding the IP address of the currently
     * connected server. Is empty if #isConnected is false.
     */
    Q_PROPERTY(
        QString connectedIP READ _getConnectedIP NOTIFY connectedIPChanged)

    /**
     * @brief Readonly property holding the number of received bytes while
     * connected to the current server. 0 if #isConnected is false. Formatting
     * the number is the responsibiltiy of the UI.
     */
    Q_PROPERTY(
        qint64 receivedBytes READ _getReceivedBytes NOTIFY receivedBytesChanged)

    /**
     * @brief Readonly property holding the number of sent bytes while
     * connected to the current server. 0 if #isConnected is false. Formatting
     * the number is the responsibiltiy of the UI.
     */
    Q_PROPERTY(qint64 sentBytes READ _getSentBytes NOTIFY sentBytesChanged)

    Q_PROPERTY(
        qint64 uptimeSeconds READ _getUptimeSeconds NOTIFY uptimeSecondsChanged)

    /**
     * @brief Readonly property holding the list of currently available
     * countries. The list gets refreshed periodically, since e.g. changes in
     * the settings (like "obfuscated" on/off) have influence on this list.
     */
    Q_PROPERTY(
        QVariant countryList READ _getCountryList NOTIFY countryListChanged)

    /**
     * @brief Readonly property holding a list of server groups the currently
     * connected server is part of. This list is empty if #isConnected is false.
     * @details The list consists of integers that are equal to the integer
     * values of the #Group enum.
     */
    Q_PROPERTY(QVariantList connectedServerGroups READ _getConnectedServerGroups
                   NOTIFY connectedServerGroupsChanged)

    /**
     * @brief Readonly property holding a list of server groups the currently
     * connecting server is part of. This list is empty if #isConnected is
     * false.
     * @details The list consists of integers that are equal to the integer
     * values of the #Group enum.
     */
    Q_PROPERTY(
        QVariantList connectingServerGroups READ _getConnectingServerGroups
            NOTIFY connectingServerGroupsChanged)

  public slots: // NOLINT(readability-redundant-access-specifiers)
    /**
     * @brief Connect to a random server.
     * @details Calls to this function are ignored while
     * #areConnectionCommandsPaused is true. This function should only be
     * used while #isDisconnected is true.
     */
    void quickConnect();

    /**
     * @brief Connect to a server in a specific country.
     * @param id The countries ID as specified in the #countryList.
     * @details Calls to this function are ignored while
     * #areConnectionCommandsPaused is true. This function should only be
     * used while #isDisconnected is true.
     */
    void connectToCountryById(quint32 id);

    /**
     * @brief Connect to a server in a specific city.
     * @param id The cities ID as specified in the #countryList.
     * @details Calls to this function are ignored while
     * #areConnectionCommandsPaused is true. This function should only be
     * used while #isDisconnected is true.
     */
    void connectToCityById(quint32 id);

    /**
     * @brief Connect to a specific server.
     * @param serverId The servers ID as specified by the returns values of
     * #getServers() or #getSpecialtyServers().
     * @details Calls to this function are ignored while
     * #areConnectionCommandsPaused is true. This function should only be
     * used while #isDisconnected is true.
     */
    void connectToServerById(quint32 serverId);

    /**
     * @brief Connect to a server within a specific server group.
     * @param groupId The groups ID which has to be the integer value of an item
     * from the #Group enum.
     * @details Calls to this function are ignored while
     * #areConnectionCommandsPaused is true. This function should only be
     * used while #isDisconnected is true.
     */
    void connectToSpecialtyGroup(quint32 groupId);

    /**
     * @brief Connect to a server within a specific server group and country.
     * @param id The countries ID as specified in the #countryList.
     * @param groupId he groups ID which has to be the integer value of an item
     * from the #Group enum.
     * @details Calls to this function are ignored while
     * #areConnectionCommandsPaused is true. This function should only be
     * used while #isDisconnected is true.
     */
    void connectToCountryByIdAndGroup(quint32 id, quint32 groupId);

    /**
     * @brief Cancel a running connection attempt.
     * @details This tried to kill the process responsible for establishing the
     * connection. Since killing the process is not always successful, this will
     * perform a disconnect operation as well. This function should only be
     * used while #isConnecting is true.
     */
    void cancelConnection();

    /**
     * @brief Disconnect from the currently connected server.
     * @details Calls to this function are ignored while
     * #areConnectionCommandsPaused is true. This function should only be used
     * while #isConnected is true.
     */
    void disconnect();

    /**
     * @brief Get a list of available servers.
     * @param countryId The countries ID as specified by the #countryList.
     * @param cityId The cities ID as specified by the #countryList. May be <0
     * to include all cities.
     * @return A filtered list of server. The list is not only filtered by
     * `countryId` and `cityId` but also by the current settings.
     */
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): not supported by moc
    QVariantList getServers(qint32 countryId, qint32 cityId);

    /**
     * @brief Get a list of countries that have servers within a specific server
     * group.
     * @param groupId The ID if the group. This is the integer value if an item
     * of the #Group enum.
     * @return A filtered list of countries.
     */
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): not supported by moc
    QVariantList getSpecialtyCountries(quint32 groupId);

    /**
     * @brief Get a list of servers within a specialty server group and country.
     * @param groupId The ID if the group. This is the integer value if an item
     * of the #Group enum.
     * @param countryId The ID of the country as specified by the #countryList.
     * May be <0 to include all countries.
     * @return A filtered list of servers.
     */
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): not supported by moc
    QVariantList getSpecialtyServers(quint32 groupId, qint32 countryId);

    /**
     * @brief Rate the last connection.
     * @param rating An integer value between #config::consts::RATING_MIN and
     * #config::consts::RATING_MAX where larger numbers imply better rating.
     * @details This function should only be used while #isRatingPossible is
     * true.
     */
    static void rate(quint8 rating);

  signals:
    /**
     * @brief Signal the gets emitted when #areConnectionCommandsPaused is being
     * updated.
     */
    void areConnectionCommandsPausedChanged(bool);

    /**
     * @brief Signal the gets emitted when #isDisconnected is being updated.
     */
    void disconnectedChanged(bool);

    /**
     * @brief Signal the gets emitted when #isConnecting is being updated.
     */
    void connectingChanged(bool);

    /**
     * @brief Signal the gets emitted when #isConnected is being updated.
     */
    void connectedChanged(bool);

    /**
     * @brief Signal the gets emitted when #isRatingPossible is being updated.
     */
    void isRatingPossibleChanged(bool);

    /**
     * @brief Signal the gets emitted when #connectingCountryId is being
     * updated.
     */
    void connectingCountryIdChanged(qint32);

    /**
     * @brief Signal the gets emitted when #connectedCountryId is being updated.
     */
    void connectedCountryIdChanged(qint32);

    /**
     * @brief Signal the gets emitted when #connectedCityId is being updated.
     */
    void connectedCityIdChanged(qint32);

    /**
     * @brief Signal the gets emitted when #connectedServerNr is being updated.
     */
    void connectedServerNrChanged(qint32);

    /**
     * @brief Signal the gets emitted when #connectedIP is being updated.
     */
    void connectedIPChanged(QString);

    /**
     * @brief Signal the gets emitted when #receivedBytes is being updated.
     */
    void receivedBytesChanged(qint64);

    /**
     * @brief Signal the gets emitted when #sentBytes is being updated.
     */
    void sentBytesChanged(qint64);

    /**
     * @brief Signal the gets emitted when #uptimeSeconds is being updated.
     */
    void uptimeSecondsChanged(qint64);

    /**
     * @brief Signal the gets emitted when #countryList is being updated.
     */
    void countryListChanged(QVariant);

    /**
     * @brief Signal the gets emitted when #connectedServerGroups is being
     * updated.
     */
    void connectedServerGroupsChanged(QVariantList);

    /**
     * @brief Signal the gets emitted when #connectingServerGroups is being
     * updated.
     */
    void connectingServerGroupsChanged(QVariantList);

  private:
    /**
     * @brief Controller for server related information exchange.
     */
    ServerController &_serverController = ServerController::getInstance();

    /**
     * @brief Controller for country related information exchange.
     */
    CountryController &_countryController = CountryController::getInstance();

    /**
     * @brief Controller for propagating connection commands.
     */
    ConnectionController &_connectionController =
        ConnectionController::getInstance();

    /**
     * @brief Controller to observe for updates about the connection status.
     * @see #updateConnectionInfo()
     */
    StatusController &_statusController = StatusController::getInstance();

    /**
     * @brief List of currently available countries.
     */
    std::vector<Country> _countries;

    /**
     * @brief Field backing the #areConnectionCommandsPaused property.
     */
    bool _areConnectionCommandsPaused = false;

    /**
     * @brief Getter for the #areConnectionCommandsPaused property.
     */
    auto _getAreConnectionCommandsPaused() const -> bool;

    /**
     * @brief Setter for the #areConnectionCommandsPaused property.
     */
    void _setAreConnectionCommandsPaused(bool value);

    /**
     * @brief Field backing the #isDisconnected property.
     */
    bool _isDisconnected = false;

    /**
     * @brief Getter for the #isDisconnected property.
     */
    auto _getIsDisconnected() const -> bool;

    /**
     * @brief Setter for the #isDisconnected property.
     */
    void _setIsDisconnected(bool value);

    /**
     * @brief Field backing the #isConnecting property.
     */
    bool _isConnecting = false;

    /**
     * @brief Getter for the #isConnecting property.
     */
    auto _getIsConnecting() const -> bool;

    /**
     * @brief Setter for the #isConnecting property.
     */
    void _setIsConnecting(bool value);

    /**
     * @brief Field backing the #isConnected property.
     */
    bool _isConnected = false;

    /**
     * @brief Getter for the #isConnected property.
     */
    auto _getIsConnected() const -> bool;

    /**
     * @brief Setter for the #isConnected property.
     */
    void _setIsConnected(bool value);

    /**
     * @brief Field backing the #isRatingPossible property.
     */
    bool _isRatingPossible = false;

    /**
     * @brief Getter for the #isRatingPossible property.
     */
    auto _getIsRatingPossible() const -> bool;

    /**
     * @brief Setter for the #isRatingPossible property.
     */
    void _setIsRatingPossible(bool value);

    /**
     * @brief Field backing the #connectingCountryId property.
     */
    int32_t _connectingCountryId = -1;

    /**
     * @brief Getter for the #connectingCountryId property.
     */
    auto _getConnectingCountryId() const -> qint32;

    /**
     * @brief Setter for the #connectingCountryId property.
     */
    void _setConnectingCountryId(qint32 value);

    /**
     * @brief Field backing the #connectedCountryId property.
     */
    int32_t _connectedCountryId = -1;

    /**
     * @brief Getter for the #connectedCountryId property.
     */
    auto _getConnectedCountryId() const -> qint32;

    /**
     * @brief Setter for the #connectedCountryId property.
     */
    void _setConnectedCountryId(qint32 value);

    /**
     * @brief Field backing the #connectedCityId property.
     */
    int32_t _connectedCityId = -1;

    /**
     * @brief Getter for the #connectedCityId property.
     */
    auto _getConnectedCityId() const -> qint32;

    /**
     * @brief Setter for the #connectedCityId property.
     */
    void _setConnectedCityId(qint32 value);

    /**
     * @brief Field backing the #connectedServerNr property.
     */
    int32_t _connectedServerNr = 0;

    /**
     * @brief Getter for the #connectedServerNr property.
     */
    auto _getConnectedServerNr() const -> qint32;

    /**
     * @brief Setter for the #connectedServerNr property.
     */
    void _setConnectedServerNr(qint32 value);

    /**
     * @brief Field backing the #connectedIP property.
     */
    std::string _connectedIP;

    /**
     * @brief Getter for the #connectedIP property.
     */
    auto _getConnectedIP() -> QString;

    /**
     * @brief Setter for the #connectedIP property.
     */
    void _setConnectedIP(const std::string &value);

    /**
     * @brief Field backing the #receivedBytes property.
     */
    uint64_t _receivedBytes = 0;

    /**
     * @brief Getter for the #receivedBytes property.
     */
    auto _getReceivedBytes() const -> qint64;

    /**
     * @brief Setter for the #receivedBytes property.
     */
    void _setReceivedBytes(uint64_t value);

    /**
     * @brief Field backing the #sentBytes property.
     */
    uint64_t _sentBytes = 0;

    /**
     * @brief Getter for the #sentBytes property.
     */
    auto _getSentBytes() const -> qint64;

    /**
     * @brief Setter for the #sentBytes property.
     */
    void _setSentBytes(uint64_t value);

    /**
     * @brief Field backing the #uptimeSeconds property.
     */
    uint64_t _uptimeSeconds = 0;

    /**
     * @brief Getter for the #uptimeSeconds property.
     */
    auto _getUptimeSeconds() const -> qint64;

    /**
     * @brief Setter for the #uptimeSeconds property.
     */
    void _setUptimeSeconds(uint64_t value);

    /**
     * @brief Getter for the #countryList property.
     */
    auto _getCountryList() -> QVariant;

    /**
     * @brief Field backing the #connectedServerGroups property.
     */
    std::vector<Group> _connectedServerGroups;

    /**
     * @brief Getter for the #connectedServerGroups property.
     */
    auto _getConnectedServerGroups() -> QVariantList;

    /**
     * @brief Setter for the #connectedServerGroups property.
     */
    void _setConnectedServerGroups(std::vector<Group> groups);

    /**
     * @brief Field backing the #connectingServerGroups property.
     */
    std::vector<Group> _connectingServerGroups;

    /**
     * @brief Getter for the #connectingServerGroups property.
     */
    auto _getConnectingServerGroups() -> QVariantList;

    /**
     * @brief Setter for the #connectingServerGroups property.
     */
    void _setConnectingServerGroups(std::vector<Group> groups);

    /**
     * @brief Implements IConnectionInfoSubscription::updateConnectionInfo() to
     * receive updates about the connection status and related information.
     * @param newInfo The new connection information.
     */
    void updateConnectionInfo(const ConnectionInfo &newInfo) override;

    /**
     * @brief Implements ICountriesSubscription::updateCountryList() to receive
     * updates about the list of available countries.
     * @param countryList The updated list of countries.
     * @see #countryList
     */
    void updateCountryList(const std::vector<Country> &countryList) override;
};

#endif // CONNECTIONMEDIATOR_HPP
