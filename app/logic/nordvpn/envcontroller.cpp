#include "envcontroller.h"

EnvController &EnvController::getInstance() {
    static EnvController instance;
    return instance;
}

EnvInfo EnvController::getEnvInfo() {
    EnvInfo envInfo;
    envInfo.shellAvailable = this->_isShellAvailable();
    envInfo.nordvpnInstalled = this->_isNordvpnInstalled();
    envInfo.internetConnected = this->_isInternetConnected();
    if (envInfo.internetConnected == true) {
        envInfo.loggedIn = this->_isLoggedIn();
        if (envInfo.loggedIn == false)
            // double check, because for some unknown reason it sometimes
            // returns false erroneously
            envInfo.loggedIn = this->_isLoggedIn();
    }
    return std::move(envInfo);
}

void EnvController::setLoggedIn(bool loggedIn) {
    this->_envInfo.loggedIn = loggedIn;
    this->_notifySubscribers();
}

bool EnvController::_isInternetConnected() {
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
    for (uint8_t i = 0; i < testUrls.size(); i++) {
        long httpCode = 0;
        std::string httpData;
        CURL *curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, testUrls[i].c_str());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 8);
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

bool EnvController::_isShellAvailable() {
    // execute the no-op command that has to exit successfully
    return Process::execute(":").success();
}

bool EnvController::_isNordvpnInstalled() {
    auto result = Process::execute("nordvpn --version");
    return result.success() && result.output.rfind("NordVPN Version", 0) == 0;
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

void EnvController::startBackgroundTask() {
    this->_performBackgroundTask = true;
    // create and run new daemon thread
    std::thread(&EnvController::_backgroundTask, this).detach();
}

void EnvController::stopBackgroundTask() {
    this->_performBackgroundTask = false;
}

void EnvController::_backgroundTask() {
    // get complete env info only once
    this->_envInfo = this->getEnvInfo();
    this->_notifySubscribers();
    int i = 0;
    // then periodically do partial updates
    while (this->_performBackgroundTask) {
        i++;
        this->_envInfo.internetConnected = this->_isInternetConnected();
        this->_envInfo.shellAvailable = this->_isShellAvailable();
        this->_envInfo.nordvpnInstalled = this->_isNordvpnInstalled();
        if (this->_envInfo.internetConnected == true && i == 30) {
            i = 0;
            /*
             * Calling "nordvpn account" probably performs a request to the
             * NordVPN API. The number of request is limited to 250 per hour
             * (empirically tested on 2020-10-31). Hence, the loggedIn property
             * gets updated only once per minute (= 60 request per hour) to
             * leave enough scope for unknown changes on the part of the
             * NordVPN API.
             */
            this->_envInfo.loggedIn = this->_isLoggedIn();
            if (this->_envInfo.loggedIn == false)
                // double check, because for some unknown reason it sometimes
                // returns false erroneously
                this->_envInfo.loggedIn = this->_isLoggedIn();
        }
        this->_notifySubscribers();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}
