#ifndef RECENTSCONTROLLER_H
#define RECENTSCONTROLLER_H

#include <cstdint>
#include <vector>

#include "basecontroller.h"
#include "common/templates/subscribable.h"
#include "data/models/country.h"
#include "logic/subscriptions/irecentssubscription.h"

/**
 * @brief The RecentsController class is responsible for managing a list of
 * recent connections.
 */
class RecentsController : public BaseController,
                          public Subscribable<IRecentsSubscription> {
    // Singleton:
    // https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
  public:
    RecentsController(const RecentsController &) = delete;
    void operator=(const RecentsController &) = delete;
    RecentsController(RecentsController &&) = delete;
    auto operator=(RecentsController &&) -> RecentsController & = delete;
    ~RecentsController() = default;

    /**
     * @brief Get the singleton instance of RecentsController.
     * @details The instance will be constructed if it does not exist already.
     */
    static auto getInstance() -> RecentsController &;
    /**
     * @brief Get a list of all countries available.
     */
    static auto getRecentCountries() -> std::vector<Country>;

    /**
     * @brief Add a country to the list of recently connected countries by ID.
     */
    void addTooRecentsList(uint32_t countryId);

    /**
     * @brief Remove a country from the list of recently connected countries by
     * ID.
     */
    void removeFromRecentsList(uint32_t countryId);

  private:
    RecentsController();

    /**
     * @brief Internal list of all recently connected countries.
     */
    std::vector<Country> _recents;

    /**
     * @brief Implementation of Subscribable::notifySubscriber().
     * @param subscriber
     */
    void notifySubscriber(IRecentsSubscription &subscriber) override;
};

#endif // RECENTSCONTROLLER_H
