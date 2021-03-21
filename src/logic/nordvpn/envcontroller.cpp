#include "envcontroller.hpp"

#include <chrono>
#include <cstddef>
#include <curl/curl.h>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <utility>

#include "common/io/process.hpp"
#include "common/io/processresult.hpp"
#include "common/types/nullable.hpp"
#include "common/util/strings.hpp"
#include "config.hpp"

EnvController::EnvController() {
    this->registerBackgroundTask([this](bool tick) { _backgroundTask(tick); },
                                 [this] { _backgroundTaskInit(); },
                                 config::consts::ENV_UPDATE_INTERVAL,
                                 std::chrono::minutes(1));
}

auto EnvController::getEnvInfo() -> EnvInfo {
    EnvInfo envInfo;
    envInfo.shellAvailable = EnvController::_isShellAvailable();
    envInfo.nordvpnInstalled = EnvController::_isNordvpnInstalled();
    envInfo.internetConnected = EnvController::_isInternetConnected();
    envInfo.miscError = EnvController::_checkMiscError();
    if (envInfo.internetConnected) {
        envInfo.loggedIn = EnvController::_isLoggedIn();
        if (envInfo.loggedIn == false) {
            // double check, because for some unknown reason it sometimes
            // returns false erroneously
            envInfo.loggedIn = EnvController::_isLoggedIn();
        }
    }
    return std::move(envInfo);
}

void EnvController::setLoggedIn(bool loggedIn) {
    this->_envInfo.loggedIn = loggedIn;
    this->notifySubscribers();
}

auto EnvController::_isInternetConnected() -> bool {
    // Use curl to check if any of the following servers respond.
    // The last one (ncsi) is used by Windows to check if the computer is
    // connected. It requests a text file whichs content may be checked against.
    std::vector<std::string> testUrls = {
        // Google DNS
        "https://8.8.8.8",
        // Cloudflare DNS
        "https://1.1.1.1",
        // Microsoft's Network Connectivity Status Indicator
        "http://www.msftncsi.com/ncsi.txt",
    };
    bool connected = false;
    for (size_t i = 0; i < testUrls.size(); i++) {
        std::string httpData;
        CURL *curl = curl_easy_init();
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        curl_easy_setopt(curl, CURLOPT_URL, testUrls[i].c_str());
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 8);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        curl_easy_setopt(
            curl, CURLOPT_WRITEFUNCTION,
            *[](const char *in, size_t size, size_t num,
                std::string *out) -> size_t {
                size_t totalBytes = size * num;
                out->append(in, totalBytes);
                return totalBytes;
            });
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &httpData);
        CURLcode curlCode = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (i == testUrls.size() - 1) {
            connected = (util::string::trim(httpData) == "Microsoft NCSI");
        } else {
            connected = (curlCode == CURLE_OK);
        }
        if (connected) {
            return true;
        }
    }
    return false;
}

auto EnvController::_isShellAvailable() -> bool {
    // execute the no-op command that has to exit successfully
    auto result = Process::execute(":");
    return result.success();
}

auto EnvController::_isNordvpnInstalled() -> bool {
    auto result = Process::execute("nordvpn --version");
    return result.success() && result.output.rfind("NordVPN Version", 0) == 0;
}

auto EnvController::_isLoggedIn() -> Nullable<bool> {
    auto result = Process::execute("nordvpn account");
    auto out = util::string::toLower(result.output);
    if (result.success()) {
        return true;
    }
    if (!std::regex_search(out, std::regex("not logged in")) &&
        std::regex_search(out, std::regex("email.+@"))) {
        return false;
    }
    return Nullable<bool>();
}

auto EnvController::_checkMiscError() -> std::string {
    auto result = Process::execute("nordvpn status");
    if (result.success()) {
        return "";
    }
    // nordvpn might write it's error message to stdout instead of stdin, who
    // knows ...
    return !result.error.empty() ? result.error : result.output;
}

void EnvController::notifySubscriber(IEnvInfoSubscription &subscriber) {
    subscriber.updateEnv(this->_envInfo);
}

void EnvController::_backgroundTaskInit() {
    // get complete env info only once
    this->_envInfo = EnvController::getEnvInfo();
    this->notifySubscribers();
}

void EnvController::_backgroundTask(bool isSpecialTick) {
    this->_envInfo.internetConnected = this->_isInternetConnected();
    this->_envInfo.shellAvailable = this->_isShellAvailable();
    this->_envInfo.nordvpnInstalled = this->_isNordvpnInstalled();
    this->_envInfo.miscError = this->_checkMiscError();
    if (this->_envInfo.internetConnected && isSpecialTick) {
        /*
         * Calling "nordvpn account" probably performs a request to the
         * NordVPN API. The number of request is limited to 250 per hour
         * (empirically tested on 2020-10-31). Hence, the loggedIn property
         * gets updated only once per minute (= 60 request per hour) to
         * leave enough scope for unknown changes on the part of the
         * NordVPN API.
         * See registerBackgroundTask(...) to change the isSpecialTick interval.
         */
        this->_envInfo.loggedIn = this->_isLoggedIn();
        if (this->_envInfo.loggedIn == false) {
            // double check, because for some unknown reason it sometimes
            // returns false erroneously
            this->_envInfo.loggedIn = this->_isLoggedIn();
        }
    }
    this->notifySubscribers();
}
