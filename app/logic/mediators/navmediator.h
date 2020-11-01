#ifndef NAVMEDIATOR_H
#define NAVMEDIATOR_H

#include <map>
#include <string>

#include <QObject>
#include <QString>

#include "logic/nordvpn/envcontroller.h"

enum class MainWindowView {
    Startup,
    Main,
    Login,
    NoConnection,
    NoShell,
    NotInstalled,
};

class NavMediator : public QObject, public IEnvInfoSubscription {
    Q_OBJECT

    Q_PROPERTY(QString mainWindowViewSource READ _getMainWindowViewSource NOTIFY
                   mainWindowViewSourceChanged)

  public:
    NavMediator();

  public slots:
    void showViewAfterSuccessfulLogin();

  signals:
    void mainWindowViewSourceChanged(QString);

  private:
    std::map<MainWindowView, std::string> _viewSourceMap{
        {MainWindowView::Startup, /**/ "qrc:/ui/views/StartupView.qml"},
        {MainWindowView::Main, /*   */ "qrc:/ui/views/MainView.qml"},
        {MainWindowView::Login, /*  */ "qrc:/ui/views/LoginView.qml"},
        {MainWindowView::NoConnection, "qrc:/ui/views/NoConnectionView.qml"},
        {MainWindowView::NoShell, /**/ "qrc:/ui/views/NoShellView.qml"},
        {MainWindowView::NotInstalled, "qrc:/ui/views/NotInstalledView.qml"},
    };
    MainWindowView _currentMainWindowView = MainWindowView::Startup;
    QString _getMainWindowViewSource();
    void _setCurrentMainWindowView(MainWindowView v);

    void updateEnv(const EnvInfo &envInfo) override;
};

#endif // NAVMEDIATOR_H
