#ifndef PREFERENCESCONTROLLER_H
#define PREFERENCESCONTROLLER_H

#include "basecontroller.h"
#include "logic/models/nordvpnsettings.h"

// Singleton:
// https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
class PreferencesController : public BaseController {
  public:
    PreferencesController(const PreferencesController &) = delete;
    void operator=(const PreferencesController &) = delete;
    static PreferencesController &getInstance();
    NordVpnSettings getNordvpnSettings();
    void updateNordvpnSettings(const NordVpnSettings &settings);
    NordVpnSettings restoreDefaultNordvpnSettings();

  private:
    PreferencesController() {}
};

#endif // PREFERENCESCONTROLLER_H
