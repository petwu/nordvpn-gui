#ifndef PREFERENCESCONTROLLER_HPP
#define PREFERENCESCONTROLLER_HPP

#include "basecontroller.hpp"
#include "nordvpn/ipreferencescontroller.hpp"
#include "nordvpn/models/nordvpnsettings.hpp"

class PreferencesController : public virtual IPreferencesController,
                              public BaseController {
  public:
    auto getNordvpnSettings() -> NordVpnSettings override;
    void updateNordvpnSettings(const NordVpnSettings &settings) override;
    auto restoreDefaultNordvpnSettings() -> NordVpnSettings override;
};

#endif // PREFERENCESCONTROLLER_HPP
