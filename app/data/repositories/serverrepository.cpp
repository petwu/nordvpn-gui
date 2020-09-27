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

std::string Country::toString(bool oneLine) {
    std::string sb = oneLine ? " " : "\n  ";
    std::string s = oneLine ? ", " : "\n  ";
    std::string se = oneLine ? " " : "\n";
    auto r =
        std::string("Country {") + sb + "id = " + std::to_string(this->id) + s +
        "name = " + this->name + s + "lat = " + std::to_string(this->lat) + s +
        "lng = " + std::to_string(this->lng) + s +
        "offsetLeft = " + std::to_string(this->offsetLeft) + s +
        "offsetTop = " + std::to_string(this->offsetTop) + s + "cities = [" + s;
    for (uint8_t i = 0; i < this->cities.size(); i++) {
        r += (oneLine ? "" : "  ") + this->cities[i].toString(true) +
             (i < this->cities.size() - 1 ? s : se);
    }
    r += (oneLine ? "]" : "  ]") + se + "}";
    return r;
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
                if (server.country.name != country["name"])
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

std::vector<Country> ServerRepository::fetchCountries() {
    std::vector<Country> countries;
    std::string httpReponse =
        BaseRepository::curl("https://api.nordvpn.com/v1/servers/countries");
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
