#include "updatemediator.hpp"

auto UpdateMediator::_getIsUpdateAvailable() -> bool {
    // TODO
    /* return UpdateCheckRepository::getLatestVersion() >
           UpdateCheckRepository::getCurrentVersion(); */
    return false;
}

auto UpdateMediator::_getAppVersion() -> QString {
    // TODO
    // return
    // QString(UpdateCheckRepository::getCurrentVersion().toString().c_str());
    return "";
}

auto UpdateMediator::_getLatestVersion() -> QString {
    // TODO
    // return
    // QString(UpdateCheckRepository::getLatestVersion().toString().c_str());
    return "";
}
