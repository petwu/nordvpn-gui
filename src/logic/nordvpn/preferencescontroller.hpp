#ifndef PREFERENCESCONTROLLER_HPP
#define PREFERENCESCONTROLLER_HPP

#include "basecontroller.hpp"
#include "logic/models/nordvpnsettings.hpp"

/**
 * @brief The PreferencesController class is responsible for retrieving and
 * saving settings/preferences. These include both, preferences regarding the
 * NordVPN CLI and preferences specific to this application.
 */
class PreferencesController : public BaseController {
    // Singleton:
    // https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
  public:
    /**
     * @brief Disable copy constructor (part of the singleton implementation).
     */
    PreferencesController(const PreferencesController &) = delete;

    /**
     * @brief Disable copy assignment (part of the singleton implementation).
     */
    void operator=(const PreferencesController &) = delete;

    PreferencesController(PreferencesController &&) = default;
    auto operator=(PreferencesController &&)
        -> PreferencesController & = default;
    ~PreferencesController() = default;

    /**
     * @brief Get the singleton instance of PreferencesController.
     * @details The instance will be constructed if it does not exist already.
     */
    static auto getInstance() -> PreferencesController &;

    /**
     * @brief Retrieve the current settings regarding the NordVPN CLI.
     */
    static auto getNordvpnSettings() -> NordVpnSettings;

    /**
     * @brief Update the settings of the NordVPN CLI based on the difference of
     * the provided `settings` and the current settings (getNordvpnSettings()).
     */
    void updateNordvpnSettings(const NordVpnSettings &settings);

    /**
     * @brief Restore the default settings of the NordVPN CLI.
     * @details The NordVPN CLI provides built-in functionlity for this by
     * calling `nordvpn set defaults`.
     * @warning The user will be logged out by this operation and has to
     * re-enter his/her credentials afterwards.
     */
    auto restoreDefaultNordvpnSettings() -> NordVpnSettings;

  private:
    /**
     * @brief Empty, private contructor (part of the singleton implementation).
     */
    PreferencesController() = default;
};

#endif // PREFERENCESCONTROLLER_HPP
