#ifndef ICOUNTRIESSUBSCRIPTION_HPP
#define ICOUNTRIESSUBSCRIPTION_HPP

/**
 * @brief ICountriesSubscription is an interface that can be implemented by any
 * class that wants updates of the country list from the #ServerController.
 *
 * @details Call #ServerController::attach() and possibly
 * #ServerController::startBackgroundTasks() to start reveciving updates.
 */
class ICountriesSubscription {
  public:
    /**
     * @brief Function that is called by the #ServerController every time there
     * is an update of the country list.
     */
    virtual void updateCountryList(const std::vector<Country> &countryList) = 0;
};

#endif // ICOUNTRIESSUBSCRIPTION_HPP
