#include "envcontroller.h"

std::string EnvInfo::toString() {
    return std::string("EnvInfo {") +                       //
           "\n  internetConnected   = " +                   //
           (this->internetConnected ? "true" : "false") +   //
           "\n  nordvpnApiAvailable = " +                   //
           (this->nordvpnApiAvailable ? "true" : "false") + //
           "\n  shellAvailable      = " +                   //
           (this->shellAvailable ? "true" : "false") +      //
           "\n  nordvpnInstalled    = " +                   //
           (this->nordvpnInstalled ? "true" : "false") +    //
           "\n  loggedIn            = " +                   //
           (this->loggedIn ? "true" : "false") +            //
           "\n}";
}

EnvController &EnvController::getInstance() {
    static EnvController instance;
    return instance;
}

EnvInfo EnvController::getEnvInfo() {
    EnvInfo envInfo;
    envInfo.internetConnected = this->_isInternetConnected();
    envInfo.nordvpnApiAvailable = this->_isNordvpnApiAvailable();
    envInfo.shellAvailable = this->_isShellAvailable();
    envInfo.nordvpnInstalled = this->_isNordvpnInstalled();
    envInfo.loggedIn = this->_isLoggedIn();
    return std::move(envInfo);
}

bool EnvController::_isInternetConnected() {
    // Use curl to check if any of the following servers respond.
    // The last one (ncsi) is used by Windows to check if the computer is
    // connected. It requests a text file whichs content may be checked against.
    std::vector<std::string> testUrls = {
        // Google DNS
        "https://8.8.8.8",
        "https://8.8.4.4",
        // Cloudflare DNS
        "https://1.1.1.1",
        "https://1.0.0.1",
        // Microsoft's Network Connectivity Status Indicator
        "http://www.msftncsi.com/ncsi.txt",
    };
    bool connected = false;
    for (uint8_t i = 0; i < testUrls.size(); i++) {
        long httpCode = 0;
        std::string httpData;
        CURL *curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, testUrls[i].c_str());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(
            curl, CURLOPT_WRITEFUNCTION,
            *[](const char *in, size_t size, size_t num,
                std::string *out) -> size_t {
                size_t totalBytes = size * num;
                out->append(in, totalBytes);
                return totalBytes;
            });
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &httpData);
        CURLcode curlCode = curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
        curl_easy_cleanup(curl);
        if (i == testUrls.size() - 1) {
            connected = (util::string::trim(httpData) == "Microsoft NCSI");
        } else
            connected = (curlCode == CURLE_OK && httpCode == 200);
        if (connected)
            return true;
    }
    return false;
}

bool EnvController::_isNordvpnApiAvailable() {
    // try to request data from one API endpoint:
    // https://api.nordvpn.com/v1/technologies was chosen because it returns a
    // relatively small amount of data (< 1 kiB as of 2020-10-18) that is
    // unlikely to increase
    long httpCode = 0;
    std::string httpData;
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL,
                     "https://api.nordvpn.com/v1/technologies");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(
        curl, CURLOPT_WRITEFUNCTION,
        *[](const char *in, size_t size, size_t num,
            std::string *out) -> size_t {
            size_t totalBytes = size * num;
            out->append(in, totalBytes);
            return totalBytes;
        });
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &httpData);
    CURLcode curlCode = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);
    return curlCode == CURLE_OK && httpCode == 200 && httpData != "";
}

bool EnvController::_isShellAvailable() {
    // execute the no-op command that has to exit successfully
    return Process::execute(":").success();
}

bool EnvController::_isNordvpnInstalled() {
    auto result = Process::execute("nordvpn --version");
    return result.exitCode == 0 &&
           result.output.rfind("NordVPN Version", 0) == 0;
}

bool EnvController::_isLoggedIn() {
    auto result = Process::execute("nordvpn account");
    auto out = util::string::toLower(result.output);
    if (!result.success())
        return false;
    if (std::regex_search(out, std::regex("not logged in")))
        return false;
    if (std::regex_search(out, std::regex("email.+@")))
        return true;
    return false;
}

void EnvController::attach(IEnvInfoSubscription *subscriber) {
    this->_subscribers.push_back(subscriber);
}

void EnvController::detach(IEnvInfoSubscription *subscriber) {
    this->_subscribers.erase(std::remove(this->_subscribers.begin(),
                                         this->_subscribers.end(), subscriber),
                             this->_subscribers.end());
}

void EnvController::_notifySubscribers() {
    for (auto &subscriber : this->_subscribers) {
        if (subscriber != nullptr)
            subscriber->updateEnv(this->_envInfo);
    }
}
