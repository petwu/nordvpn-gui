#ifndef PREFERENCESMEDIATOR_H
#define PREFERENCESMEDIATOR_H

#include <QObject>
#include <QString>
#include <QVariant>

#include "logic/nordvpn/preferencescontroller.h"
#include "qmldataconverter.h"

/**
 * @brief The PreferencesMediator class is responsible for communicating
 * preferences between the UI and the logic components.
 *
 * @attention The following functions/slots only modify the in-memory settings
 * object, hence the made changes are not propagated to the CLI immediately.
 * - setAutoconnect()
 * - setCybersec()
 * - setDns()
 * - setDnsAddress()
 * - setKillswitch()
 * - setNotify()
 * - setObfuscated()
 * - setProtocol()
 * - setTechnology()
 * - addSubnetToWhitelist()
 * - updateWhitelistSubnet()
 * - removeSubnetFromWhitelist()
 * - addPortsToWhitelist()
 * - updateWhitelistPorts()
 * - removePortsFromWhitelist()
 * .
 * This allows for the settings to be discarded easily with refreshSettings().
 * Use saveNordvpnSettings() to save the made changes.
 */
class PreferencesMediator : public QObject {
    Q_OBJECT

    /**
     * @brief Object representing the current settings of the NordVPN CLI.
     */
    Q_PROPERTY(QVariant nordvpnSettings READ _getNordvpnSettings NOTIFY
                   nordvpnSettingsChanged)

  public:
    /**
     * @brief Default PreferencesMediator constructor.
     */
    PreferencesMediator();

  public slots:
    /**
     * @brief Function to trigger the update of the N settings from the NordVPN
     * CLI.
     * @details The updated settings will get propagated to the UI.
     */
    void refreshSettings();

    /**
     * @brief Activate or deactivate autoconnect.
     * @param enabled True/False to enable/disable autoconnect.
     */
    void setAutoconnect(bool enabled);

    /**
     * @brief Activate or deactivate cybersec.
     * @param enabled True/False to enable/disable cybersec.
     */
    void setCybersec(bool enabled);

    /**
     * @brief Activate or deactivate custom DNS.
     * @param enabled True/False to enable/disable custom DNS.
     */
    void setDns(bool enabled);

    /**
     * @brief Set the value of the `i`-th custom DNS address.
     * @details Settings any DNS address to a non-empty value will enable DNS
     * automatically, hence no call to setDns(true) is required.
     * @param i The DNS addresses index.
     * @param addr The custom DNS address value.
     */
    void setDnsAddress(int i, QString addr);

    /**
     * @brief Activate or deactivate the killswitch.
     * @param enabled True/False to enable/disable the killswitch.
     */
    void setKillswitch(bool enabled);

    /**
     * @brief Activate or deactivate system notification.
     * @param enabled True/False to enable/disable system notification.
     */
    void setNotify(bool enabled);

    /**
     * @brief Activate or deactivate obfuscated.
     * @param enabled True/False to enable/disable obfuscated.
     */
    void setObfuscated(bool enabled);

    /**
     * @brief The the network protocol to use. See #Protocol for appropriate
     * values.
     * @param protocol A string matching a #Protocol value. Unknown values don't
     * have any effect.
     */
    void setProtocol(QString protocol);

    /**
     * @brief Set the connection technology to use. See #Technology for
     * appropriate values.
     * @param technology
     */
    void setTechnology(QString technology);
    void addSubnetToWhitelist();
    void updateWhitelistSubnet(int index, QString subnet);
    void removeSubnetFromWhitelist(int index);
    void addPortsToWhitelist();
    void updateWhitelistPorts(int index, QString portFrom, QString portTo,
                              uint protocolFlag);
    void removePortsFromWhitelist(int index);

    void saveNordvpnSettings();
    void restoreDefaultNordvpnSettings();

    bool isValidIpAddress(QString ip);
    bool isValidSubnetMask(QString subnet);
    bool isValidPort(QString port);

  signals:
    void nordvpnSettingsChanged(QVariantMap);

  private:
    PreferencesController &_preferencesController =
        PreferencesController::getInstance();
    NordVpnSettings _nordvpnSettings;
    QVariantMap _getNordvpnSettings();
    void _nordvpnSettingsChanged();
};

#endif // PREFERENCESMEDIATOR_H
