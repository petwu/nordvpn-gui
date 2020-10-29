#include "navmediator.h"

QString NavMediator::_getViewSource() {
    auto viewSource = QString(this->_viewSourceMap[this->_currentView].c_str());
    return std::move(viewSource);
}

void NavMediator::_setCurrentView(View v) {
    this->_currentView = v;
    this->viewSourceChanged(this->_getViewSource());
}
