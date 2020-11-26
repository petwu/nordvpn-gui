#ifndef RECENTSMEDIATOR_H
#define RECENTSMEDIATOR_H

#include <QObject>
#include <QVariantList>

#include "logic/nordvpn/servercontroller.h"
#include "qmldataconverter.h"

/**
 * @brief The RecentsMediator class is responsible for providing the UI with a
 * list of recently connected countries. It also provides necessary functions to
 * allow modification of the list.
 */
class RecentsMediator : public QObject, public ICountriesSubscription {
    Q_OBJECT

  public:
    /**
     * @brief Default constructor.
     */
    RecentsMediator();

    /**
     * @brief Readonly property that holds the list of recently connected
     * countries and notified the UI about updates.
     */
    Q_PROPERTY(QVariantList recentCountries READ _getRecentCountries NOTIFY
                   recentCountriesChanged)

  public slots:
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
    ServerController &_serverController = ServerController::getInstance();

    /**
     * @brief Internal list or recently connected countries.
     */
    std::vector<Country> _recentCountries;

    /**
     * @brief Getter for the #recentCountries property.
     * @return
     */
    QVariantList _getRecentCountries();

    /**
     * @brief Implements ICountriesSubscription::updateRecents() to receive
     * updates of the recents list.
     * @param newRecents The updated list or recently connected countries.
     */
    void updateRecents(const std::vector<Country> &newRecents) override;

    /**
     * @brief Implements ICountriesSubscription::updateCountryList(). Empty
     * implementation since this information is not required.
     */
    void updateCountryList(const std::vector<Country> &countryList) override {}
};

#endif // RECENTSMEDIATOR_H
