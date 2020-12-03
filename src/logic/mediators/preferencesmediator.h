#ifndef PREFERENCESMEDIATOR_H
#define PREFERENCESMEDIATOR_H

#include <QObject>
#include <QString>
#include <QVariant>

#include "logic/nordvpn/preferencescontroller.h"
#include "qmldataconverter.h"

/**
 * @brief PORT_MAX is the maximum value, a port may have. A port is a 16-bit
 * number and hence allows values from 0 to 65535 (= 2^16 - 1).
 */
constexpr uint PORT_MAX = 65535;

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
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): Qt is out of scope
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

  public slots: // NOLINT(readability-redundant-access-specifiers)
    /**
     * @brief Function to trigger the update of the settings from the NordVPN
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
    void setDnsAddress(int i, const QString &addr);

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
    void setProtocol(const QString &protocol);

    /**
     * @brief Set the connection technology to use. See #Technology for
     * appropriate values.
     * @param technology
     */
    void setTechnology(const QString &technology);
    void addSubnetToWhitelist();
    void updateWhitelistSubnet(int index, const QString &subnet);
    void removeSubnetFromWhitelist(int index);
    void addPortsToWhitelist();
    void updateWhitelistPorts(int index, const QString &portFrom,
                              const QString &portTo, uint protocolFlag);
    void removePortsFromWhitelist(int index);

    void saveNordvpnSettings();
    void restoreDefaultNordvpnSettings();

    // NOLINTNEXTLINE(modernize-use-trailing-return-type): not supported by moc
    static bool isValidIpAddress(const QString &ip);
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): not supported by moc
    static bool isValidSubnetMask(const QString &subnet);
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): not supported by moc
    static bool isValidPort(const QString &port);

  signals:
    void nordvpnSettingsChanged(QVariantMap);

  private:
    PreferencesController &_preferencesController =
        PreferencesController::getInstance();
    NordVpnSettings _nordvpnSettings;
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): not supported by moc
    QVariantMap _getNordvpnSettings();
    void _nordvpnSettingsChanged();
};

#endif // PREFERENCESMEDIATOR_H
