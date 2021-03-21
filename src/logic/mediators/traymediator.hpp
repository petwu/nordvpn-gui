#ifndef TRAYMEDIATOR_HPP
#define TRAYMEDIATOR_HPP

#include <QAction>
#include <QByteArrayData>
#include <QMenu>
#include <QObject>
#include <QString>
#include <QSystemTrayIcon>
#include <QtGlobal>
#include <memory>
#include <vector>

#include "data/models/country.hpp"
#include "logic/models/connectioninfo.hpp"
#include "logic/nordvpn/icountrycontroller.hpp"
#include "logic/nordvpn/irecentscontroller.hpp"
#include "logic/nordvpn/istatuscontroller.hpp"
#include "logic/subscriptions/iconnectioninfosubscription.hpp"
#include "logic/subscriptions/icountriessubscription.hpp"
#include "logic/subscriptions/irecentssubscription.hpp"

/**
 * @brief The TrayMediator class is a simple mediator to interact with a tray
 * icon for the application. The mediator is only responsible for instantiating
 * the tray icon object and its context menu. Everything else is supposed to be
 * handles in QMl files. This includes:
 *
 * - Providing the actual icon image via #setIconSource.
 * - Acting on signals that are emitted when the tray icon or a context menu
 *   action gets triggered. See all members below the 'signals:' label for more
 *   information.
 *
 * @details This class can only be instantiated once (= de-facto singleton),
 * since only one tray icon should exists at the same time.
 *
 * @note The is also a pure QML type named SystemTrayIcon
 * (https://doc.qt.io/qt-5/qml-qt-labs-platform-systemtrayicon.html), but at the
 * time of writing it was still part of the experimental Qt Labs module and
 * didn't e.g. support displaying icons inside the context menu.
 */
class TrayMediator : public QObject,
                     public IConnectionInfoSubscription,
                     public ICountriesSubscription,
                     public IRecentsSubscription {
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): Qt is out of scope
    Q_OBJECT

  public:
    TrayMediator(std::shared_ptr<ICountryController> countryController,
                 std::shared_ptr<IRecentsController> recentsController,
                 std::shared_ptr<IStatusController> statusController);
    TrayMediator(const TrayMediator &) = delete;
    auto operator=(const TrayMediator &) -> TrayMediator & = delete;
    TrayMediator(TrayMediator &&) = delete;
    auto operator=(TrayMediator &&) -> TrayMediator & = delete;
    ~TrayMediator() override;

  public slots: // NOLINT(readability-redundant-access-specifiers)
    /**
     * @brief Changes the try icon and shows it if not visible yet.
     * @param filename Path or QRC resource to the icon.
     */
    void setIconSource(const QString &filename);

  signals:
    /**
     * @brief Toggle the main windows visibility. Triggered, when the user
     * clicks on the tray icon itself.
     */

    void toggleMainWindowAction();
    /**
     * @brief Make the main window visible.
     * Triggerd by a context menu action.
     */

    void showMainWindowAction();
    /**
     * @brief Open the settings/preferences window.
     * Triggerd by a context menu action.
     */

    void openPreferencesWindowAction();
    /**
     * @brief Quit the application. This means really quitting it, not just
     * minimize it to tray.
     * Triggerd by a context menu action.
     */
    void quitApplicationAction();

    /**
     * @brief Quick connect to a server.
     * Triggerd by a context menu action.
     */
    void quickConnectAction();

    /**
     * @brief Abort the current connection establishment.
     * Triggerd by a context menu action.
     */
    void cancelConnectingAction();

    /**
     * @brief Disconnect from the current server.
     * Triggerd by a context menu action.
     */
    void disconnectAction();

    /**
     * @brief Connect to a specific country by ID.
     * Triggerd by a context menu action.
     */
    void connectToCountryById(qint32);

  private:
    const std::shared_ptr<ICountryController> _countryController;
    const std::shared_ptr<IRecentsController> _recentsController;
    const std::shared_ptr<IStatusController> _statusController;

    /**
     * @brief Object holding the tray icon.
     */
    QSystemTrayIcon _trayIcon;

    /**
     * @brief The tray icons context menu. (opens on right-click)
     */
    std::unique_ptr<QMenu> _trayContextMenu;

    /**
     * @brief Submenu of #_trayContextMenu to list recent connections.
     */
    std::unique_ptr<QMenu> _recentsSubmenu;

    /**
     * @brief Submenu of #_trayContextMenu to list all countries.
     */
    std::unique_ptr<QMenu> _countriesSubmenu;

    /**
     * @brief Passive menu action to show the current connection status
     * (connected/disconnected/etc.).
     * @details QActions are owned by the QMenu and don't need to be detroyed
     * manually.
     */
    QAction *_statusAction;

    /**
     * @brief Menu action to quick connect.
     * @details QActions are owned by the QMenu and don't need to be detroyed
     * manually.
     */
    QAction *_quickConnectAction;

    /**
     * @brief Menu action to abort the current connection establishment.
     * @details QActions are owned by the QMenu and don't need to be detroyed
     * manually.
     */
    QAction *_cancelConnectingAction;

    /**
     * @brief Menu action to disconnect.
     * @details QActions are owned by the QMenu and don't need to be detroyed
     * manually.
     */
    QAction *_disconnectAction;

    /**
     * @brief Implements IConnectionInfoSubscription::updateConnectionInfo() to
     * receive updates about the connection status and related information.
     * @param newInfo The new connection information.
     */
    void updateConnectionInfo(const ConnectionInfo &newInfo) override;

    /**
     * @brief Implements ICountriesSubscription::updateRecents() to receive
     * updates of the recents list.
     * @param newRecents The updated list or recently connected countries.
     */
    void updateRecents(const std::vector<Country> &newRecents) override;

    /**
     * @brief Implements ICountriesSubscription::updateCountryList().
     */
    void updateCountryList(const std::vector<Country> &countryList) override;
};

#endif // TRAYMEDIATOR_HPP
