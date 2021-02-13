#ifndef UPDATEMEDIATOR_H
#define UPDATEMEDIATOR_H

#include <QObject>
#include <QString>

#include "common/types/version.h"

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
    bool _getIsUpdateAvailable();
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): not supported by moc
    QString _getAppVersion();
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): not supported by moc
    QString _getLatestVersion();
};

#endif // UPDATEMEDIATOR_H
