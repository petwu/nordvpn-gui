#ifndef MAINWINDOWVIEW_HPP
#define MAINWINDOWVIEW_HPP

/**
 * @brief The MainWindowView enum identifies the view that should fill the
 * applications main window.
 * @details There are QML views for every enum value located at
 * ui/views/<enum-value>View.qml respectively.
 */
enum class MainWindowView {
    Startup, ///< View while starting the app.
    Main,    ///< Default view that enables the user to connect to a VPN server
             ///< (via map, etc.).
    Login,   ///< View that allows the user to enter his/her login credentials.
    NoConnection, ///< Error view, in case of no internet connection.
    NoShell, ///< Error view, in case no shell commands can be executed (see
             ///< #popen3()).
    NotInstalled, ///< Error view, in case the nordvpn CLI is not installed or
                  ///< not in $PATH.
    MiscError,    ///< Error view, in case the is something wrong that cannot be
                  ///< narrowed down to any specific error cause.
};

#endif // MAINWINDOWVIEW_HPP
