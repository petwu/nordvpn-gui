#ifndef IPREFERENCESCONTROLLER_HPP
#define IPREFERENCESCONTROLLER_HPP

#include "logic/models/nordvpnsettings.hpp"

/**
 * @brief The IPreferencesController interface is responsible for retrieving and
 * saving settings/preferences. These include both, preferences regarding the
 * NordVPN CLI and preferences specific to this application.
 */
class IPreferencesController {
  public:
    /**
     * @brief Retrieve the current settings regarding the NordVPN CLI.
     */
    virtual auto getNordvpnSettings() -> NordVpnSettings = 0;

    /**
     * @brief Update the settings of the NordVPN CLI based on the difference of
     * the provided `settings` and the current settings (getNordvpnSettings()).
     */
    virtual void updateNordvpnSettings(const NordVpnSettings &settings) = 0;

    /**
     * @brief Restore the default settings of the NordVPN CLI.
     * @details The NordVPN CLI provides built-in functionlity for this by
     * calling `nordvpn set defaults`.
     * @warning The user will be logged out by this operation and has to
     * re-enter his/her credentials afterwards.
     */
    virtual auto restoreDefaultNordvpnSettings() -> NordVpnSettings = 0;
};

#endif // IPREFERENCESCONTROLLER_HPP
