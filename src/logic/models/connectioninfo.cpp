#include "connectioninfo.h"

#include <vector>

#include "nlohmann/json.hpp"

using json = nlohmann::ordered_json;

auto ConnectionInfo::isEmpty() const -> bool {
    return this->status == ConnectionStatus::Disconnected &&
           this->server.empty() && this->serverNr == 0 &&
           this->country.empty() && this->countryId == -1 &&
           this->cityId == -1 && this->city.empty() && this->ip.empty() &&
           this->technology == Technology::Undefined &&
           this->connectionType == Protocol::Undefined && this->sent == 0 &&
           this->received == 0 && this->uptime == 0;
}

auto ConnectionInfo::toJSON() const -> std::string {
    std::vector<std::string> groups;
    for (Group g : this->groups) {
        groups.push_back(group2string(g));
    }
    json j = {
        {"status", connectionStatusToString(this->status)},
        {"server", this->server},
        {"serverNr", this->serverNr},
        {"country", this->country},
        {"countryId", this->countryId},
        {"city", this->city},
        {"cityId", this->cityId},
        {"ip", this->ip},
        {"technology", technologyToString(this->technology)},
        {"connectionType", protocolToString(this->connectionType)},
        {"sent", this->sent},
        {"received", this->received},
        {"uptime", this->uptime},
        {"load", this->load},
        {"groups", groups},
    };
    return j.dump(2);
}
