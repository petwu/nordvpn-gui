#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include "basecontroller.h"
#include "logic/models/nordvpnsettings.h"

// Singleton:
// https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
class SettingsController : public BaseController {
  public:
    SettingsController(const SettingsController &) = delete;
    void operator=(const SettingsController &) = delete;
    static SettingsController &getInstance();
    NordVpnSettings getSettings();
    void updateSettings(const NordVpnSettings &settings);

  private:
    SettingsController() {}
};

#endif // SETTINGSCONTROLLER_H
