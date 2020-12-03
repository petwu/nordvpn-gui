#include "connectioninfo.h"

auto ConnectionInfo::isEmpty() const -> bool {
    return this->status == ConnectionStatus::Disconnected &&
           this->server.empty() && this->serverNr == 0 &&
           this->country.empty() && this->countryId == -1 &&
           this->cityId == -1 && this->city.empty() && this->ip.empty() &&
           this->technology == Technology::Undefined &&
           this->connectionType == Protocol::Undefined && this->sent == 0 &&
           this->received == 0 && this->uptime == 0;
}
