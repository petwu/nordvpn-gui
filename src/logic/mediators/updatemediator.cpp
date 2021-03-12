#include "updatemediator.h"

#include <string>

#include "common/types/version.h"
#include "data/repositories/updatecheckrepository.h"

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
