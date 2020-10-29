#ifndef NAVMEDIATOR_H
#define NAVMEDIATOR_H

#include <map>
#include <string>

#include <QObject>
#include <QString>

enum class View {
    Main,
    Login,
    Preferences,
    NoConnection,
    NotInstalled,
};

class NavMediator : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString viewSource READ _getViewSource NOTIFY viewSourceChanged)

  public slots:
    /*
    void openSettings();
    void closeSettings();
    */

  signals:
    void viewSourceChanged(QString);

  private:
    std::map<View, std::string> _viewSourceMap{
        {View::Main, "qrc:/ui/views/MainView.qml"},
    };
    View _currentView = View::Main;
    QString _getViewSource();
    void _setCurrentView(View v);
};

#endif // NAVMEDIATOR_H
