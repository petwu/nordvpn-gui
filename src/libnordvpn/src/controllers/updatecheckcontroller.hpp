#ifndef UPDATECHECKCONTROLLER_HPP
#define UPDATECHECKCONTROLLER_HPP

#include <memory>
#include <vector>

#include "backgroundtaskable.hpp"
#include "basecontroller.hpp"
#include "nordvpn/iupdatecheckcontroller.hpp"
#include "nordvpn/models/version.hpp"
#include "nordvpn/subscriptions/iupdatechecksubscription.hpp"
#include "repositories/iupdatecheckrepository.hpp"
#include "subscribable.hpp"

/**
 * @brief The UpdateCheckController class provides version information and
 * notifies about potentially available updates.
 */
class UpdateCheckController : public virtual IUpdateCheckController,
                              public BaseController,
                              public Subscribable<IUpdateCheckSubscription>,
                              public BackgroundTaskable {
  public:
    UpdateCheckController(
        std::shared_ptr<IUpdateCheckRepository> updateCheckRepository);

    auto getCurrentVersion() -> Version override;
    auto getLatestVersion() -> Version override;
    auto getAvailableUpdateVersion() -> Version override;
    void setIsUpdateHidden(Version v, bool isHidden) override;

  private:
    std::shared_ptr<IUpdateCheckRepository> _updateCheckRepository;
    bool _isUpdateAvailable = false;
    bool _isUpdateHidden = false;
    Version _currentVersion = Version::invalid();
    Version _availableVersion = Version::invalid();

    /**
     * @brief The background task that is run periodically to check for updates.
     */
    void _backgroundTask();

    /**
     * @brief Implementation of Subscribable::notifySubscriber().
     */
    void notifySubscriber(IUpdateCheckSubscription &subscriber) override;
};

#endif // UPDATECHECKCONTROLLER_HPP
