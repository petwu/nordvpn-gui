#include "devmediator.h"

DevMediator::DevMediator() { //
    this->_statusController.attach(this);
}

void DevMediator::updateConnectionInfo(const ConnectionInfo &newInfo) {
    this->_connectionInfo = newInfo.toJSON();
    this->connectionInfoChanged(QString(this->_connectionInfo.c_str()));
}

auto DevMediator::_getConnectionInfo() -> QString {
    return QString(this->_connectionInfo.c_str());
}

void DevMediator::toggleDebugInformation() {
#if IS_DEBUG
    this->_showDebugInformation = !this->_showDebugInformation;
    this->showDebugInformationChanged(this->_showDebugInformation);
#endif
}

auto DevMediator::_getShowDebugInformation() const -> bool {
#if IS_DEBUG
    return this->_showDebugInformation;
#else
    return false;
#endif
}
