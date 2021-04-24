#ifndef RECENTSMEDIATOR_HPP
#define RECENTSMEDIATOR_HPP

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QtCore>
#include <memory>
#include <vector>

#include "nordvpn/irecentscontroller.hpp"
#include "nordvpn/models/country.hpp"
#include "nordvpn/subscriptions/irecentssubscription.hpp"

/**
 * @brief The RecentsMediator class is responsible for providing the UI with a
 * list of recently connected countries. It also provides necessary functions to
 * allow modification of the list.
 */
class RecentsMediator : public QObject, public IRecentsSubscription {
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): Qt is out of scope
    Q_OBJECT

  public:
    /**
     * @brief Default constructor.
     */
    RecentsMediator(std::shared_ptr<IRecentsController> recentsController);

    /**
     * @brief Readonly property that holds the list of recently connected
     * countries and notified the UI about updates.
     */
    Q_PROPERTY(QVariantList recentCountries READ _getRecentCountries NOTIFY
                   recentCountriesChanged)

  public slots: // NOLINT(readability-redundant-access-specifiers)
    /**
     * @brief Remove a country from the list of recently connected countries by
     * its ID.
     */
    void removeFromRecentsList(quint32 countryId);

  signals:
    /**
     * @brief Signal that gets emitted once the list of recently connected
     * countries has changed.
     */
    void recentCountriesChanged(QVariantList);

  private:
    /**
     * @brief Controller, that provides that necessary data.
     */
    const std::shared_ptr<IRecentsController> _recentsController;

    /**
     * @brief Internal list or recently connected countries.
     */
    std::vector<Country> _recentCountries;

    /**
     * @brief Getter for the #recentCountries property.
     * @return
     */
    auto _getRecentCountries() -> QVariantList;

    /**
     * @brief Implements ICountriesSubscription::updateRecents() to receive
     * updates of the recents list.
     * @param newRecents The updated list or recently connected countries.
     */
    void updateRecents(const std::vector<Country> &newRecents) override;
};

#endif // RECENTSMEDIATOR_HPP
