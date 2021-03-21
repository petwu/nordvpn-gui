#ifndef DEVMEDIATOR_HPP
#define DEVMEDIATOR_HPP

#include <QByteArrayData>
#include <QObject>
#include <QString>
#include <memory>
#include <string>

#include "logic/models/connectioninfo.hpp"
#include "logic/nordvpn/istatuscontroller.hpp"
#include "logic/subscriptions/iconnectioninfosubscription.hpp"

/**
 * @brief The DevMediator class provides utility properties and slots for
 * development purposes only.
 */
class DevMediator : public QObject, public IConnectionInfoSubscription {
    // NOLINTNEXTLINE(modernize-use-trailing-return-type): Qt is out of scope
    Q_OBJECT

  public:
    DevMediator(std::shared_ptr<IStatusController> statusController);

    /**
     * @brief Property that indicates that some controls should show debug
     * information, e.g. as some kind of overlay.
     *
     * @details For Release builds this property is always false.
     */
    Q_PROPERTY(bool showDebugInformation READ _getShowDebugInformation NOTIFY
                   showDebugInformationChanged)

    /**
     * @brief Property that holds a JSON string of data about the currenct
     * connection status.
     */
    Q_PROPERTY(QString connectionInfo READ _getConnectionInfo NOTIFY
                   connectionInfoChanged)

  public slots: // NOLINT(readability-redundant-access-specifiers)
    /**
     * @brief Toggle the showDebugInformation property from true/false to
     * false/true.
     */
    void toggleDebugInformation();

  signals:
    /**
     * @brief Signal that gets emitted once the showDebugInformation property
     * changes it's value.
     */
    void showDebugInformationChanged(bool);

    /**
     * @brief Signal that gets emitted once the connection info string changes.
     */
    void connectionInfoChanged(QString);

  private:
    const std::shared_ptr<IStatusController> _statusController;

    /**
     * @brief Implements IConnectionInfoSubscription::updateConnectionInfo() to
     * receive updates about the connection status and related information.
     * @param newInfo The new connection information.
     */
    void updateConnectionInfo(const ConnectionInfo &newInfo) override;

    /**
     * @brief Backing attribute for the #connectionInfo property.
     */
    std::string _connectionInfo = "{}";

    /**
     * @brief Getter for _connectionInfo.
     */
    auto _getConnectionInfo() -> QString;

    /**
     * @brief Backing attribute for the #showDebugInformation property.
     */
    bool _showDebugInformation = false;

    /**
     * @brief Getter for _showDebugInformation.
     */
    auto _getShowDebugInformation() const -> bool;
};

#endif // DEVMEDIATOR_HPP
