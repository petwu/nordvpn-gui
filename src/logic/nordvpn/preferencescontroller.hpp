#ifndef PREFERENCESCONTROLLER_HPP
#define PREFERENCESCONTROLLER_HPP

#include "basecontroller.hpp"
#include "ipreferencescontroller.hpp"
#include "logic/models/nordvpnsettings.hpp"

class PreferencesController : public virtual IPreferencesController,
                              public BaseController {
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

    auto getNordvpnSettings() -> NordVpnSettings override;
    void updateNordvpnSettings(const NordVpnSettings &settings) override;
    auto restoreDefaultNordvpnSettings() -> NordVpnSettings override;

  private:
    /**
     * @brief Empty, private contructor (part of the singleton implementation).
     */
    PreferencesController() = default;
};

#endif // PREFERENCESCONTROLLER_HPP
