#ifndef PREFERENCESMEDIATOR_H
#define PREFERENCESMEDIATOR_H

#include <QObject>
#include <QString>
#include <QVariant>

#include "logic/nordvpn/preferencescontroller.h"
#include "qmldataconverter.h"

class PreferencesMediator : public QObject {
    Q_OBJECT

    Q_PROPERTY(QVariant nordvpnSettings READ _getNordvpnSettings NOTIFY
                   nordvpnSettingsChanged)

  public:
    PreferencesMediator();

  public slots:
    void setAutoconnect(bool enabled);
    void setCybersec(bool enabled);
    void setDns(bool enabled);
    void setDnsAddress(int i, QString addr);
    void setKillswitch(bool enabled);
    void setNotify(bool enabled);
    void setObfuscated(bool enabled);
    void setProtocol(QString protocol);
    void setTechnology(QString technology);
    void saveNordvpnSettings();
    void restoreDefaultNordvpnSettings();
    bool isValidIpAddress(QString ip);

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
