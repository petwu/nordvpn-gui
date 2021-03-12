#ifndef UPDATEMEDIATOR_H
#define UPDATEMEDIATOR_H

#include <QByteArrayData>
#include <QObject>
#include <QString>

class UpdateMediator : public QObject {
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): Qt is out of scope
    Q_OBJECT

  public:
    Q_PROPERTY(bool isUpdateAvailable READ _getIsUpdateAvailable NOTIFY
                   isUpdateAvailableChanged)

    Q_PROPERTY(QString appVersion READ _getAppVersion)
    Q_PROPERTY(QString latestVersion READ _getLatestVersion NOTIFY
                   latestVersionChanged)

  signals: // NOLINT(readability-redundant-access-specifiers)
    void isUpdateAvailableChanged(bool);
    void latestVersionChanged(QString);

  private:
    static auto _getIsUpdateAvailable() -> bool;
    static auto _getAppVersion() -> QString;
    static auto _getLatestVersion() -> QString;
};

#endif // UPDATEMEDIATOR_H
