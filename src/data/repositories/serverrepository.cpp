#include "serverrepository.h"

#include <algorithm>
#include <initializer_list>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>

#include "baserepository.h"
#include "common/util/strings.h"
#include "config.h"
#include "data/enums/group.h"
#include "data/enums/securityprotocol.h"
#include "data/models/location.h"

auto ServerRepository::fetchServers() -> std::vector<Server> {
    std::vector<Server> servers;
    std::string httpReponse =
        BaseRepository::curl(config::urls::NORDVPN_API_ALL_SERVERS);
    if (httpReponse.empty()) {
        return fetchServersFromCache();
    }

    json j;
    try {
        j = json::parse(httpReponse);
    } catch (...) {
        return fetchServersFromCache();
    }

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
        if (s["id"].is_number_integer()) {
            server.id = json::number_integer_t(s["id"]);
        }
        if (s["name"].is_string()) {
            server.name = json::string_t(s["name"]);
        }
        if (s["hostname"].is_string()) {
            server.hostname = json::string_t(s["hostname"]);
        }
        server.connectName =
            util::string::replaceSuffix(server.hostname, ".nordvpn.com", "");
        if (s["load"].is_number_integer()) {
            server.load = json::number_integer_t(s["load"]);
        }
        if (s["locations"][0]["country"].is_object()) {
            json c = s["locations"][0]["country"];
            if (c["id"].is_number_integer()) {
                server.countryId = json::number_integer_t(c["id"]);
            }
            if (c["city"]["id"].is_number_integer()) {
                server.cityId = json::number_integer_t(c["city"]["id"]);
            }
        }
        if (s["groups"].is_array()) {
            for (json g : s["groups"]) {
                std::string identifier = json::string_t(g["identifier"]);
                if (identifier == "legacy_standard") {
                    server.groups.push_back(Group::Standard);
                } else if (identifier == "legacy_double_vpn") {
                    server.groups.push_back(Group::Double);
                } else if (identifier == "legacy_onion_over_vpn") {
                    server.groups.push_back(Group::Onion);
                } else if (identifier == "legacy_p2p") {
                    server.groups.push_back(Group::P2P);
                } else if (identifier == "legacy_obfuscated_servers") {
                    server.groups.push_back(Group::Obfuscated);
                }
            }
        }
        if (s["technologies"].is_array()) {
            for (json t : s["technologies"]) {
                std::string identifier = json::string_t(t["identifier"]);
                if (identifier == "openvpn_tcp") {
                    server.securityProtocols.push_back(
                        SecurityProtocol::OpenVPN_TCP);
                } else if (identifier == "openvpn_udp") {
                    server.securityProtocols.push_back(
                        SecurityProtocol::OpenVPN_UDP);
                } else if (identifier == "openvpn_tcp_tls_crypt") {
                    server.securityProtocols.push_back(
                        SecurityProtocol::OpenVPN_TCP_TLS_Crypt);
                } else if (identifier == "openvpn_udp_tls_crypt") {
                    server.securityProtocols.push_back(
                        SecurityProtocol::OpenVPN_UDP_TLS_Crypt);
                } else if (identifier == "openvpn_dedicated_tcp") {
                    server.securityProtocols.push_back(
                        SecurityProtocol::OpenVPN_TCP_Dedicated);
                } else if (identifier == "openvpn_dedicated_udp") {
                    server.securityProtocols.push_back(
                        SecurityProtocol::OpenVPN_UDP_Dedicated);
                } else if (identifier == "openvpn_xor_tcp") {
                    server.securityProtocols.push_back(
                        SecurityProtocol::OpenVPN_TCP_Obfuscated);
                } else if (identifier == "openvpn_xor_udp") {
                    server.securityProtocols.push_back(
                        SecurityProtocol::OpenVPN_UDP_Obfuscated);
                } else if (identifier == "wireguard_udp") {
                    server.securityProtocols.push_back(
                        SecurityProtocol::Wireguard);
                } else if (identifier == "socks") {
                    server.securityProtocols.push_back(
                        SecurityProtocol::Socks5);
                } else if (identifier == "proxy") {
                    server.securityProtocols.push_back(
                        SecurityProtocol::HTTP_Proxy);
                } else if (identifier == "proxy_cybersec") {
                    server.securityProtocols.push_back(
                        SecurityProtocol::HTTP_CyberSec_Proxy);
                } else if (identifier == "proxy_ssl") {
                    server.securityProtocols.push_back(
                        SecurityProtocol::HTTP_Proxy_SSL);
                } else if (identifier == "proxy_ssl_cybersec") {
                    server.securityProtocols.push_back(
                        SecurityProtocol::HTTP_CyberSec_Proxy_SSL);
                } else if (identifier == "ikev2") {
                    server.securityProtocols.push_back(
                        SecurityProtocol::IKEv2_IPSec);
                } else if (identifier == "ikev2_v6") {
                    server.securityProtocols.push_back(
                        SecurityProtocol::IKEv2_IPSec_IPv6);
                } else if (identifier == "openvpn_tcp_v6") {
                    server.securityProtocols.push_back(
                        SecurityProtocol::OpenVPN_TCP_IPv6);
                } else if (identifier == "openvpn_udp_v6") {
                    server.securityProtocols.push_back(
                        SecurityProtocol::OpenVPN_UDP_IPv6);
                }
            }
        }
        servers.push_back(std::move(server));
    }
    saveServerListToFile(servers);
    std::map<Group, int> groupDistribution;
    groupDistribution[Group::Standard] = 0;
    groupDistribution[Group::Double] = 0;
    groupDistribution[Group::Onion] = 0;
    groupDistribution[Group::P2P] = 0;
    groupDistribution[Group::Obfuscated] = 0;
    int nServers = 0;
    for (const auto &s : servers) {
        nServers++;
        for (auto g : s.groups) {
            groupDistribution[g]++;
        }
    }

    return std::move(servers);
}

auto ServerRepository::fetchServersFromCache() -> std::vector<Server> {
    std::string file =
        BaseRepository::readFile(config::paths::serversCacheJson());
    std::vector<Server> servers;
    if (!file.empty()) {
        json arr = json::parse(file);
        if (arr.is_array()) {
            for (const auto &elem : arr) {
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
        if (i < n - 1) {
            file += ",";
        }
    }
    file += "]";
    BaseRepository::writeFile(config::paths::serversCacheJson(), file);
}

auto ServerRepository::fetchCountries() -> std::vector<Country> {
    std::vector<Country> countries;
    std::string httpReponse =
        BaseRepository::curl(config::urls::NORDVPN_API_ALL_COUNTRIES);
    if (httpReponse.empty()) {
        return fetchCountriesFromCache();
    }

    json j;
    try {
        j = json::parse(httpReponse);
    } catch (...) {
        return fetchCountriesFromCache();
    }

    if (!j.is_array()) {
        return std::move(countries);
    }
    std::string fileContent =
        BaseRepository::readFile(config::paths::countriesJson());
    json myCountries =
        !fileContent.empty() ? json::parse(fileContent) : json::array();
    if (!myCountries.is_array()) {
        myCountries = json::array();
    }

    for (auto c : j) {
        Country country;
        if (c["id"].is_number_integer()) {
            country.id = json::number_integer_t(c["id"]);
        }
        if (c["name"].is_string()) {
            country.name = json::string_t(c["name"]);
        }
        if (c["code"].is_string()) {
            country.countryCode =
                util::string::toLower(json::string_t(c["code"]));
        }
        for (auto myC : myCountries) {
            if (country.name != myC["name"]) {
                continue;
            }
            if (myC["connectName"].is_string()) {
                country.connectName = json::string_t(myC["connectName"]);
            }
            if (myC["lat"].is_number_float()) {
                country.lat = json::number_float_t(myC["lat"]);
            }
            if (myC["lng"].is_number_float()) {
                country.lng = json::number_float_t(myC["lng"]);
            }
            if (myC["offsetLeft"].is_number_float()) {
                country.offsetLeft = json::number_float_t(myC["offsetLeft"]);
            }
            if (myC["offsetTop"].is_number_float()) {
                country.offsetTop = json::number_float_t(myC["offsetTop"]);
            }
        }
        if (!c["cities"].is_array()) {
            continue;
        }
        for (auto y : c["cities"]) {
            Location city;
            if (y["id"].is_number_integer()) {
                city.id = json::number_integer_t(y["id"]);
            }
            if (y["name"].is_string()) {
                city.name = json::string_t(y["name"]);
            }
            if (y["latitude"].is_number_float()) {
                city.lat = json::number_float_t(y["latitude"]);
            }
            if (y["longitude"].is_number_float()) {
                city.lng = json::number_float_t(y["longitude"]);
            }
            for (auto myC : myCountries) {
                if (!myC["cities"].is_array()) {
                    continue;
                }
                for (auto myCC : myC["cities"]) {
                    if (myCC["name"] == city.name &&
                        myCC["connectName"].is_string()) {
                        city.connectName = json::string_t(myCC["connectName"]);
                        break;
                    }
                }
            }
            country.cities.push_back(city);
        }
        countries.push_back(country);
    }
    saveCountryListToFile(countries);

    return countries;
}

auto ServerRepository::fetchCountriesFromCache() -> std::vector<Country> {
    std::string file =
        BaseRepository::readFile(config::paths::countriesCacheJson());
    std::vector<Country> countries;
    if (!file.empty()) {
        json arr = json::parse(file);
        if (arr.is_array()) {
            for (const auto &elem : arr) {
                countries.push_back(Country::fromJSON(elem.dump()));
            }
        }
    }
    return std::move(countries);
}

void ServerRepository::saveCountryListToFile(
    std::vector<Country> &countryList) {
    std::string file = "[";
    auto n = countryList.size();
    for (int i = 0; i < n; i++) {
        file += countryList[i].toJSON();
        if (i < n - 1) {
            file += ",";
        }
    }
    file += "]";
    BaseRepository::writeFile(config::paths::countriesCacheJson(), file);
}
