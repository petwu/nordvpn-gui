#ifndef TRAYMEDIATOR_H
#define TRAYMEDIATOR_H

#include <iostream>

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
 * @note The is also a pure QML type names SystemTrayIcon
 * (https://doc.qt.io/qt-5/qml-qt-labs-platform-systemtrayicon.html), but at the
 * time of writing it was still part of the experimental Qt Labs module and
 * didn't e.g. support displaying icons inside the context menu.
 */
class TrayMediator : public QObject {
    Q_OBJECT

  public:
    TrayMediator();
    ~TrayMediator();

  public slots:
    /**
     * @brief Changes the try icon and shows it if not visible yet.
     * @param filename Path or QRC ressource to the icon.
     */
    void setIconSource(QString filename);

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
    /** @brief Object holding the tray icon. */
    QSystemTrayIcon _trayIcon;
    /** @brief The tray icons context menu. (opens on right-click) */
    QMenu *_trayContextMenu;
};

#endif // TRAYMEDIATOR_H
