#ifndef UPDATEMEDIATOR_HPP
#define UPDATEMEDIATOR_HPP

#include <QByteArrayData>
#include <QObject>
#include <QString>
#include <memory>

#include "nordvpn/iupdatecheckcontroller.hpp"
#include "nordvpn/models/version.hpp"
#include "nordvpn/subscriptions/iupdatechecksubscription.hpp"

class UpdateMediator : public QObject, public IUpdateCheckSubscription {
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): Qt is out of scope
    Q_OBJECT

  public:
    UpdateMediator(
        std::shared_ptr<IUpdateCheckController> updateCheckController);

    /**
     * @brief Flag that is true, if an update is available and should be
     * presented to the user.
     */
    Q_PROPERTY(bool showUpdatePanel READ _getShowUpdatePanel NOTIFY
                   showUpdatePanelChanged)

  public slots: // NOLINT(readability-redundant-access-specifiers)
    /**
     * @brief Shows a dialog that either show the newest available version and
     * a link to directions to install it or a notification that states that the
     * latest version is already installed, depending on whether an update is
     * available or not.
     */
    void showUpdateDialog();

  signals: // NOLINT(readability-redundant-access-specifiers)
    void showUpdatePanelChanged(bool);
    void currentVersionChanged(QString);
    void latestVersionChanged(QString);

  private:
    /**
     * @brief Controller to get the update status from and delegate actions to.
     */
    std::shared_ptr<IUpdateCheckController> _updateCheckController;

    bool _isUpdateAvailable = false;
    bool _isUpdateHidden = false;
    bool _isUpdateHiddenTemp = false;
    Version _currentVersion = Version::invalid();
    Version _availableVersion = Version::invalid();

    /**
     * @brief Getter for the isUpdateAvailable property.
     */
    auto _getShowUpdatePanel() const -> bool;

    /**
     * @brief Implementation of IUpdateCheckSubscription::updateUpdateStatus().
     */
    void updateUpdateStatus(bool isUpdateAvailable, bool isUpdateHidden,
                            Version currentVersion,
                            Version availableVersion) override;
};

#endif // UPDATEMEDIATOR_HPP
