#include "serverrepository.h"

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

std::string Server::toJSON() {
    json j;
    j["id"] = this->id;
    j["name"] = this->name;
    j["hostname"] = this->hostname;
    j["connectName"] = this->connectName;
    j["load"] = this->load;
    j["countryId"] = this->countryId;
    j["cityId"] = this->cityId;
    for (auto g : this->groups)
        j["groups"].push_back(g);
    return j.dump();
}

Server Server::fromJSON(const std::string &s) {
    Server server;
    json j = json::parse(s);
    if (j["id"].is_number_integer())
        server.id = json::number_integer_t(j["id"]);
    if (j["name"].is_string())
        server.name = json::string_t(j["name"]);
    if (j["hostname"].is_string())
        server.hostname = json::string_t(j["hostname"]);
    if (j["connectName"].is_string())
        server.connectName = json::string_t(j["connectName"]);
    if (j["load"].is_number_integer())
        server.load = json::number_integer_t(j["load"]);
    if (j["countryId"].is_number_integer())
        server.countryId = json::number_integer_t(j["countryId"]);
    if (j["cityId"].is_number_integer())
        server.cityId = json::number_integer_t(j["cityId"]);
    if (j["groups"].is_array()) {
        for (auto g : j["groups"])
            if (g.is_number_integer())
                server.groups.push_back(
                    static_cast<Group>(json::number_integer_t(g)));
    }
    return std::move(server);
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

    for (json s : j) {
        if (s["status"].is_string() &&
            json::string_t(s["status"]) != "online") {
            // ignore servers that aren't online
            continue;
        }
        Server server;
        if (s["id"].is_number_integer())
            server.id = json::number_integer_t(s["id"]);
        if (s["name"].is_string())
            server.name = json::string_t(s["name"]);
        if (s["hostname"].is_string())
            server.hostname = json::string_t(s["hostname"]);
        server.connectName =
            util::string::replaceSuffix(server.hostname, ".nordvpn.com", "");
        if (s["load"].is_number_integer())
            server.load = json::number_integer_t(s["load"]);
        if (s["locations"][0]["country"].is_object()) {
            json c = s["locations"][0]["country"];
            if (c["id"].is_number_integer())
                server.countryId = json::number_integer_t(c["id"]);
            if (c["city"]["id"].is_number_integer())
                server.cityId = json::number_integer_t(c["city"]["id"]);
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
    saveServerListToFile(servers);

    return std::move(servers);
}

std::vector<Country> ServerRepository::fetchCountries() {
    std::vector<Country> countries;
    std::string httpReponse =
        BaseRepository::curl(config::urls::NORDVPN_API_ALL_COUNTRIES);
    json j = json::parse(httpReponse);
    if (!j.is_array()) {
        return std::move(countries);
    }
    json myCountries = getCountriesJSON();

    for (auto c : j) {
        Country country;
        if (c["id"].is_number_integer())
            country.id = json::number_integer_t(c["id"]);
        if (c["name"].is_string())
            country.name = json::string_t(c["name"]);
        if (c["code"].is_string())
            country.countryCode =
                util::string::toLower(json::string_t(c["code"]));
        for (auto myC : myCountries) {
            if (country.name != myC["name"])
                continue;
            if (myC["connectName"].is_string())
                country.connectName = json::string_t(myC["connectName"]);
            if (myC["lat"].is_number_float())
                country.lat = json::number_float_t(myC["lat"]);
            if (myC["lng"].is_number_float())
                country.lng = json::number_float_t(myC["lng"]);
            if (myC["offsetLeft"].is_number_float())
                country.offsetLeft = json::number_float_t(myC["offsetLeft"]);
            if (myC["offsetTop"].is_number_float())
                country.offsetTop = json::number_float_t(myC["offsetTop"]);
        }
        if (!c["cities"].is_array())
            continue;
        for (auto y : c["cities"]) {
            Location city;
            if (y["id"].is_number_integer())
                city.id = json::number_integer_t(y["id"]);
            if (y["name"].is_string())
                city.name = json::string_t(y["name"]);
            if (y["latitude"].is_number_float())
                city.lat = json::number_float_t(y["latitude"]);
            if (y["longitude"].is_number_float())
                city.lng = json::number_float_t(y["longitude"]);
            country.cities.push_back(city);
        }
        countries.push_back(country);
    }

    return countries;
}

std::vector<Server> ServerRepository::fetchServersFromCache() {
    std::string file =
        BaseRepository::readFile(config::paths::SERVER_CACHE_JSON);
    std::vector<Server> servers;
    if (!file.empty()) {
        json arr = json::parse(file);
        if (arr.is_array()) {
            for (auto elem : arr) {
                servers.push_back(Server::fromJSON(elem.dump()));
            }
        }
    }
    return std::move(servers);
}

void ServerRepository::saveServerListToFile(std::vector<Server> &serverList) {
    std::string file = "[";
    auto n = serverList.size();
    for (int i = 0; i < n; i++) {
        file += serverList[i].toJSON();
        if (i < n - 1)
            file += ",";
    }
    file += "]";
    BaseRepository::writeFile(config::paths::SERVER_CACHE_JSON, file);
}
