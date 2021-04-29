#include "updatecheckcontroller.hpp"

#include <cstddef>
#include <utility>

#include "nordvpn/config.hpp"

UpdateCheckController::UpdateCheckController(
    std::shared_ptr<IUpdateCheckRepository> updateCheckRepository)
    : _updateCheckRepository(std::move(updateCheckRepository)) {
    this->registerBackgroundTask([this] { _backgroundTask(); },
                                 config::consts::UPDATE_CHECK_INTERVAL);
}

auto UpdateCheckController::getCurrentVersion() -> Version {
    return this->_updateCheckRepository->getCurrentVersion();
}

auto UpdateCheckController::getLatestVersion() -> Version {
    auto allReleases = this->_updateCheckRepository->getAllReleaseVersions();
    if (allReleases.empty()) {
        return Version::invalid();
    }
    return allReleases.back();
}

auto UpdateCheckController::getAvailableUpdateVersion() -> Version {
#if !(NORDVPN_ENABLE_UPDATE_CHECK)
    return Version::invalid();
#endif
    auto allReleases = this->_updateCheckRepository->getAllReleaseVersions();
    if (allReleases.empty()) {
        return Version::invalid();
    }
    // allReleases is sorted -> take the last
    auto version = allReleases.back();
    if (!this->_updateCheckRepository->isUpdateHidden(version)) {
        return version;
    }
    // if the latest release is hidden, he propably is not interested in
    // feature releases in between the current and the latest version, but
    // he might still be interested in bugfixes for his/her current version
    auto currentVersion = this->getCurrentVersion();
    for (size_t i = allReleases.size() - 2; i >= 0; i++) {
        auto release = allReleases[i];
        if (release <= currentVersion) {
            break;
        }
        if (release.major() == currentVersion.major() &&
            release.minor() == currentVersion.minor() &&
            release.patch() > currentVersion.patch()) {
            return release;
        }
    }
    // latest version is hidden and there is no patch for the current version,
    // we return the (hidden) latest version, as the caller can still check
    // with isUpdateHidden() if the latest version is hidden
    return version;
}

void UpdateCheckController::setIsUpdateHidden(Version v, bool isHidden) {
    this->_updateCheckRepository->setIsUpdateHidden(v, isHidden);
    if (this->_isUpdateHidden != isHidden) {
        this->_isUpdateHidden = isHidden;
        this->notifySubscribers();
    }
}

void UpdateCheckController::_backgroundTask() {
    this->_currentVersion = this->getCurrentVersion();
    this->_availableVersion = this->getAvailableUpdateVersion();
    this->_isUpdateAvailable = this->_availableVersion > this->_currentVersion;
    this->_isUpdateHidden =
        this->_updateCheckRepository->isUpdateHidden(this->_availableVersion);
    this->notifySubscribers();
}

void UpdateCheckController::notifySubscriber(
    IUpdateCheckSubscription &subscriber) {
    subscriber.updateUpdateStatus(this->_isUpdateAvailable,
                                  this->_isUpdateHidden, this->_currentVersion,
                                  this->_availableVersion);
}
