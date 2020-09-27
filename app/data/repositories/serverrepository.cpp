#include "serverrepository.h"

std::string Location::toString(bool oneLine) {
    std::string sb = oneLine ? " " : "\n  ";
    std::string s = oneLine ? ", " : "\n  ";
    std::string se = oneLine ? " " : "\n";
    return std::string("Location {") + sb + "id = " + std::to_string(this->id) +
           s + "name = " + this->name + s +
           "lat = " + std::to_string(this->lat) + s +
           "lng = " + std::to_string(this->lng) + se + "}";
}

std::string group2string(Group g) {
    switch (g) {
    case Group::STANDARD:
        return "Standard";
    case Group::DOUBLE:
        return "Double";
    case Group::ONION:
        return "Onion";
    case Group::P2P:
        return "P2P";
    case Group::OBFUSCATED:
        return "Obfuscated";
    default:
        return "";
    }
}

std::string Server::toString(bool oneLine) {
    std::string sb = oneLine ? " " : "\n  ";
    std::string s = oneLine ? ", " : "\n  ";
    std::string se = oneLine ? " " : "\n";
    auto r = std::string("Server {") + sb + "name = " + this->name + s +
             "hostname = " + this->hostname + s +
             "connectName = " + this->connectName + s +
             "load = " + std::to_string(this->load) + "%" + s +
             "online = " + (this->online ? "true" : "false") + s +
             "country = " + this->country.toString() + s +
             "city = " + this->city.toString() + s + "groups = [ ";
    for (size_t i = 0; i < this->groups.size(); i++) {
        r += (i > 0 ? ", " : "") + group2string(this->groups[i]);
    }
    r += " ]" + se + "}";
    return r;
}

json::array_t ServerRepository::getCountriesJSON() {
    std::string fileContent =
        BaseRepository::readFile(config::paths::COUNTRIES_JSON);
    if (fileContent != "") {
        json data = json::parse(fileContent);
        if (data.is_array()) {
            return data;
        }
    }
    return json::array();
}

std::vector<Server> ServerRepository::fetchServers() {
    std::vector<Server> servers;
    std::string httpReponse =
        BaseRepository::curl(config::urls::NORDVPN_API_ALL_SERVERS);
    json j = json::parse(httpReponse);
    if (!j.is_array()) {
        return std::move(servers);
    }
    json countries = getCountriesJSON();

    for (json s : j) {
        Server server;
        if (s["name"].is_string())
            server.name = json::string_t(s["name"]);
        if (s["hostname"].is_string())
            server.hostname = json::string_t(s["hostname"]);
        server.connectName =
            util::string::replaceSuffix(server.hostname, ".nordvpn.com", "");
        if (s["load"].is_number_integer())
            server.load = json::number_integer_t(s["load"]);
        if (s["status"].is_string())
            server.online = (json::string_t(s["status"]) == "online");
        if (s["locations"][0]["country"].is_object()) {
            json c = s["locations"][0]["country"];
            if (c["id"].is_number_integer())
                server.country.id = json::number_integer_t(c["id"]);
            if (c["name"].is_string())
                server.country.name = json::string_t(c["name"]);
            for (auto country : countries) {
                if (server.country.name != country["statusName"])
                    continue;
                if (country["lat"].is_number_float())
                    server.country.lat = json::number_float_t(country["lat"]);
                if (country["lng"].is_number_float())
                    server.country.lng = json::number_float_t(country["lng"]);
            }
            if (c["city"]["id"].is_number_integer())
                server.city.id = json::number_integer_t(c["city"]["id"]);
            if (c["city"]["name"].is_string())
                server.city.name = json::string_t(c["city"]["name"]);
            if (c["city"]["latitude"].is_number_float())
                server.city.lat = json::number_float_t(c["city"]["latitude"]);
            if (c["city"]["longitude"].is_number_float())
                server.city.lng = json::number_float_t(c["city"]["longitude"]);
        }
        if (s["groups"].is_array()) {
            for (json g : s["groups"]) {
                std::string identifier = json::string_t(g["identifier"]);
                if (identifier == "legacy_standard") {
                    server.groups.push_back(Group::STANDARD);
                } else if (identifier == "legacy_double_vpn") {
                    server.groups.push_back(Group::DOUBLE);
                } else if (identifier == "legacy_onion_over_vpn") {
                    server.groups.push_back(Group::ONION);
                } else if (identifier == "legacy_p2p") {
                    server.groups.push_back(Group::P2P);
                } else if (identifier == "legacy_obfuscated_servers") {
                    server.groups.push_back(Group::OBFUSCATED);
                }
            }
        }
        servers.push_back(std::move(server));
    }

    return std::move(servers);
}
