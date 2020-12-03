#ifndef NAVMEDIATOR_H
#define NAVMEDIATOR_H

#include <map>
#include <string>

#include <QObject>
#include <QString>

#include "logic/enums/MainWindowView.h"
#include "logic/nordvpn/envcontroller.h"

/**
 * @brief The NavMediator class is responsible for communicating with the UI
 * about navigation that has be performed in order to display to correct view.
 */
class NavMediator : public QObject, public IEnvInfoSubscription {
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): Qt is out of scope
    Q_OBJECT

    /**
     * @brief Property that holds the value of the current view to be display in
     * the applications main window.
     */
    Q_PROPERTY(QString mainWindowViewSource READ _getMainWindowViewSource NOTIFY
                   mainWindowViewSourceChanged)

  public:
    /**
     * @brief Default NavMediator constructor.
     */
    NavMediator();

  public slots: // NOLINT(readability-redundant-access-specifiers)
    /**
     * @brief Function to tell the mediator to switch to the view that should be
     * displayed after the user entered his login credentials successfully.
     */
    void showViewAfterSuccessfulLogin();

  signals:
    /**
     * @brief Signal that gets emitted once the main windows view should be
     * changed. The QRC resource location of the new view is passed as a
     * parameter.
     */
    void mainWindowViewSourceChanged(QString);

  private:
    /**
     * @brief Map, that assigns the appropriate QRC resource location of a view
     * to every MainWindowView enum value.
     */
    std::map<MainWindowView, std::string> _viewSourceMap{
        {MainWindowView::Startup, /**/ "qrc:/ui/views/StartupView.qml"},
        {MainWindowView::Main, /*   */ "qrc:/ui/views/MainView.qml"},
        {MainWindowView::Login, /*  */ "qrc:/ui/views/LoginView.qml"},
        {MainWindowView::NoConnection, "qrc:/ui/views/NoConnectionView.qml"},
        {MainWindowView::NoShell, /**/ "qrc:/ui/views/NoShellView.qml"},
        {MainWindowView::NotInstalled, "qrc:/ui/views/NotInstalledView.qml"},
    };

    /**
     * @brief Attribute that holds the view that currently should be display in
     * the applications main window.
     */
    MainWindowView _currentMainWindowView = MainWindowView::Startup;

    /**
     * @brief Get the location of the current main windows view source.
     * @return
     */
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): not supported by moc
    QString _getMainWindowViewSource();

    /**
     * @brief Set the current main window view. This will emit the
     * #mainWindowViewSourceChanged() signal to update the UI.
     * @param v
     */
    void _setCurrentMainWindowView(MainWindowView v);

    /**
     * @brief Implements IEnvInfoSubscription::updateEnv() to get changes in the
     * environment (e.g. lost internet connection, logout via shell, etc.) that
     * might affect the currently displayed view.
     * @param envInfo Information about to environment necessary to determine
     * appropriate actions to take.
     */
    void updateEnv(const EnvInfo &envInfo) override;
};

#endif // NAVMEDIATOR_H
