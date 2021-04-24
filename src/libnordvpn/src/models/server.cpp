#include "nordvpn/models/server.hpp"

#include <algorithm>
#include <cstdint>
#include <initializer_list>
#include <nlohmann/json.hpp>
#include <utility>

#include "nordvpn/models/location.hpp"

auto Server::supportsObfuscated() -> bool {
    for (auto sp : this->securityProtocols) {
        switch (sp) {
        case SecurityProtocol::OpenVPN_TCP_Obfuscated:
        case SecurityProtocol::OpenVPN_UDP_Obfuscated:
            return true;
        default:
            break;
        }
    }
    return false;
}

auto Server::supportsCyberSec() -> bool {
    for (auto sp : this->securityProtocols) {
        switch (sp) {
        case SecurityProtocol::HTTP_CyberSec_Proxy:
        case SecurityProtocol::HTTP_CyberSec_Proxy_SSL:
            return true;
        default:
            break;
        }
    }
    return false;
}

auto Server::supportsProtocol(Protocol p) -> bool {
    for (auto sp : this->securityProtocols) {
        switch (p) {
        case Protocol::TCP:
            switch (sp) {
            case SecurityProtocol::OpenVPN_TCP:
            case SecurityProtocol::OpenVPN_TCP_IPv6:
            case SecurityProtocol::OpenVPN_TCP_TLS_Crypt:
            case SecurityProtocol::OpenVPN_TCP_Obfuscated:
            case SecurityProtocol::OpenVPN_TCP_Dedicated:
                return true;
            default:
                break;
            }
            break;
        case Protocol::UDP:
            switch (sp) {
            case SecurityProtocol::OpenVPN_UDP:
            case SecurityProtocol::OpenVPN_UDP_IPv6:
            case SecurityProtocol::OpenVPN_UDP_TLS_Crypt:
            case SecurityProtocol::OpenVPN_UDP_Obfuscated:
            case SecurityProtocol::OpenVPN_UDP_Dedicated:
                return true;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
    return false;
}

auto Server::supportsTechnology(Technology t) -> bool {
    for (auto sp : this->securityProtocols) {
        switch (t) {
        case Technology::OpenVPN:
            switch (sp) {
            case SecurityProtocol::OpenVPN_TCP:
            case SecurityProtocol::OpenVPN_UDP:
            case SecurityProtocol::OpenVPN_TCP_IPv6:
            case SecurityProtocol::OpenVPN_UDP_IPv6:
            case SecurityProtocol::OpenVPN_TCP_TLS_Crypt:
            case SecurityProtocol::OpenVPN_UDP_TLS_Crypt:
            case SecurityProtocol::OpenVPN_TCP_Obfuscated:
            case SecurityProtocol::OpenVPN_UDP_Obfuscated:
            case SecurityProtocol::OpenVPN_TCP_Dedicated:
            case SecurityProtocol::OpenVPN_UDP_Dedicated:
                return true;
            default:
                break;
            }
            break;
        case Technology::NordLynx:
            switch (sp) {
            case SecurityProtocol::Wireguard:
                return true;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
    return false;
}

auto Server::toJSON() -> std::string {
    json j;
    j["id"] = this->id;
    j["name"] = this->name;
    j["hostname"] = this->hostname;
    j["connectName"] = this->connectName;
    j["load"] = this->load;
    j["countryId"] = this->countryId;
    j["cityId"] = this->cityId;
    for (auto g : this->groups) {
        j["groups"].push_back(g);
    }
    for (auto sp : this->securityProtocols) {
        j["securityProtocols"].push_back(sp);
    }
    return j.dump();
}

auto Server::fromJSON(const std::string &s) -> Server {
    Server server;
    json j = json::parse(s);
    if (j["id"].is_number_integer()) {
        server.id = json::number_integer_t(j["id"]);
    }
    if (j["name"].is_string()) {
        server.name = json::string_t(j["name"]);
    }
    if (j["hostname"].is_string()) {
        server.hostname = json::string_t(j["hostname"]);
    }
    if (j["connectName"].is_string()) {
        server.connectName = json::string_t(j["connectName"]);
    }
    if (j["load"].is_number_integer()) {
        server.load = json::number_integer_t(j["load"]);
    }
    if (j["countryId"].is_number_integer()) {
        server.countryId = json::number_integer_t(j["countryId"]);
    }
    if (j["cityId"].is_number_integer()) {
        server.cityId = json::number_integer_t(j["cityId"]);
    }
    if (j["groups"].is_array()) {
        for (const auto &g : j["groups"]) {
            if (g.is_number_integer()) {
                server.groups.push_back(
                    static_cast<Group>(json::number_integer_t(g)));
            }
        }
    }
    if (j["securityProtocols"].is_array()) {
        for (const auto &sp : j["securityProtocols"]) {
            if (sp.is_number_integer()) {
                server.securityProtocols.push_back(
                    static_cast<SecurityProtocol>(json::number_integer_t(sp)));
            }
        }
    }
    return std::move(server);
}
