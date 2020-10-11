#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include "basecontroller.h"
#include "logic/models/nordvpnsettings.h"

class SettingsController : public BaseController {
  public:
    NordVpnSettings getSettings();
    void updateSettings(const NordVpnSettings &settings);
};

#endif // SETTINGSCONTROLLER_H
