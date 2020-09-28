#ifndef SIDEPANELMEDIATOR_H
#define SIDEPANELMEDIATOR_H

#include <QObject>
#include <QVariantList>

#include "logic/mediators/qmldataconverter.h"
#include "logic/nordvpn/servercontroller.h"

class SidePanelMediator : public QObject, public IRecentCountriesSubscription {
    Q_OBJECT

  public:
    Q_PROPERTY(QVariantList recentCountries READ _getRecentCountries NOTIFY
                   recentCountriesChanged)

  signals:
    void recentCountriesChanged(QVariantList);

  private:
    ServerController _serverController;
    std::vector<Country> _recentCountries;
    QVariantList _getRecentCountries();
    void updateRecents(const std::vector<Country> &newRecents) override;
};

#endif // SIDEPANELMEDIATOR_H
