#ifndef TRAYMEDIATOR_H
#define TRAYMEDIATOR_H

#include <iostream>
#include <memory>

#include <QAction>
#include <QMenu>
#include <QObject>
#include <QSystemTrayIcon>

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
 * @details Implemented as a singleton since only once tray icon should exists.
 *
 * @note The is also a pure QML type names SystemTrayIcon
 * (https://doc.qt.io/qt-5/qml-qt-labs-platform-systemtrayicon.html), but at the
 * time of writing it was still part of the experimental Qt Labs module and
 * didn't e.g. support displaying icons inside the context menu.
 */
class TrayMediator : public QObject {
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): Qt is out of scope
    Q_OBJECT

  public:
    /**
     * @brief Get a reference to the singleton instance of the class.
     */
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): not supported by moc
    static TrayMediator &getInstance();

    TrayMediator(const TrayMediator &) = delete;
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): not supported by moc
    TrayMediator &operator=(const TrayMediator &) = delete;

    TrayMediator(TrayMediator &&) = delete;
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): not supported by moc
    TrayMediator &operator=(TrayMediator &&) = delete;

    ~TrayMediator() override = default;

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

  private:
    TrayMediator();

    /** @brief Object holding the tray icon. */
    QSystemTrayIcon _trayIcon;
    /** @brief The tray icons context menu. (opens on right-click) */
    std::unique_ptr<QMenu> _trayContextMenu;
};

#endif // TRAYMEDIATOR_H
