#include "updatemediator.hpp"

#include <string>

#include "common/types/version.hpp"
#include "data/repositories/updatecheckrepository.hpp"

auto UpdateMediator::_getIsUpdateAvailable() -> bool {
    return UpdateCheckRepository::getLatestVersion() >
           UpdateCheckRepository::getCurrentVersion();
}

auto UpdateMediator::_getAppVersion() -> QString {
    return QString(
        UpdateCheckRepository::getCurrentVersion().toString().c_str());
}

auto UpdateMediator::_getLatestVersion() -> QString {
    return QString(
        UpdateCheckRepository::getLatestVersion().toString().c_str());
}
