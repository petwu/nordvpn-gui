/**
 * @page interaction Interaction
 *
 * @brief This page describes a simple example of how different parts of the
 * application interact with each other.
 *
 * @section The Example
 *
 * Example discussed in this pages:
 * 1. The user clicks on "Quick Connect" to connect to a random VPN server.
 * 2. The UI updates to show the current connection status.
 *
 * @section interaction_class Involved Classes
 *
 * The following class diagram shows all classes relevant to this example.
 * @note The classes, class members and dependencies are only partiallly
 * complete, just enough to help understand the example.
 *
 * @startuml
 *
 * package "src/ui/map" {
 *   file "ConnectionStatusPanel" <<QML Item>> as mapPanel {
 *   }
 * }
 *
 * package "src/logic" as logic {
 *   package mediators {
 *     class ConnectionMediator {
 *       + isConnected : bool <<Q_PROPERTY>>
 *       + isConnecting : bool <<Q_PROPERTY>>
 *       + isDisconnected : bool <<Q_PROPERTY>>
 *       + connectedCountryId : int <<Q_PROPERTY>>
 *       ...
 *       - _serverControler : ServerController
 *       - _statucController : StatusController
 *       + quickConnect() : void
 *     }
 *   }
 *   package nordvpn {
 *     class ServerController {
 *       + quickConnect() : void
 *     }
 *     class StatusController {
 *       + attach(IConnectionInfoSubscription *subscriber) : void
 *       + startBackgroundTask() : void
 *       + getStatus() : ConnectionInfo
 *       - _backgroundTask() : void
 *       - _notifySubscribers() : void
 *     }
 *     interface IConnectionInfoSubscription {
 *       + updateConnectionInfo(const ConnectionInfo &newInfo) : void
 *     }
 *   }
 * }
 *
 * package "src/common" as common {
 *   package io {
 *     class AsyncProcess {
 *       + execute(cmd : std::string, pid : *pid_t, callback :
 *              \t std::function<void(const ProcessResult)>) : void <<static>>
 *     }
 *     class Process {
 *       + execute(cmd : std::string) : ProcessResult
 *     }
 *     class ProcessResult {
 *        + command : std::string
 *        + output : std::string
 *        + error : std::string
 *        + exitCode : uint32_t
 *     }
 *   }
 * }
 *
 * mapPanel --> ConnectionMediator
 * ConnectionMediator ---> ServerController
 * ConnectionMediator --> StatusController
 * ConnectionMediator --|> IConnectionInfoSubscription
 * StatusController --> IConnectionInfoSubscription
 * ServerController -> AsyncProcess
 * StatusController -> Process
 * AsyncProcess --> ProcessResult
 * Process --> ProcessResult
 *
 * note right of mapPanel
 * "Quick Connect" button
 * end note
 *
 * @enduml
 *
 * @section interaction_sequence Interaction
 *
 * There are two separate action sequences to consider in order for the example
 * to work:
 * 1. The handling of the "Quick Connect" button.
 * 2. The update of the UI according to the new connection status.
 *
 * **Handling the "Quick Connect" button**
 *
 * 1. ConnectionStatusPanel is a QML Item that contains the "Quick Connect"
 * button.
 * 2. When the button gets presses, it calls the #ConnectionMediator.
 * 3. The mediator propagates the call to the #ServerController.
 * 4. The #ServerController uses an #AsyncProcess to call the _nordvpn_ command
 * line tool.
 * 5. The _nordvpn_ CLI handles the connection establishment.
 *
 * @startuml
 *
 * autoactivate on
 *
 * actor User
 * box ui #eef8ff
 *   participant ConnectionStatusPanel
 * end box
 * box logic #eefff8
 *   participant ConnectionMediator
 *   participant ServerController
 * end box
 * box common #ffeef8
 *   participant AsyncProcess
 * end box
 * box "\texternal\t" #f8f8f8
 *  boundary nordvpn
 * end box
 *
 * User ->> ConnectionStatusPanel : Clicks "Quick Connect"
 * ConnectionStatusPanel -> ConnectionMediator : quickConnect()
 * ConnectionMediator -> ServerController : quickConnect()
 * ServerController -> AsyncProcess : execute("nordvpn connect", ..., callback)
 * AsyncProcess ->> nordvpn : /bin/sh -c "nordvpn connect"
 * AsyncProcess --> ServerController : void
 * ServerController --> ConnectionMediator : void
 * ConnectionMediator --> ConnectionStatusPanel : void
 * deactivate ConnectionStatusPanel
 * nordvpn --> AsyncProcess : stdout + stderr
 * AsyncProcess ->> ServerController : callback(ProcessResult)
 * deactivate ServerController
 *
 * @enduml
 *
 * After this process, at first there is not UI update because of calling
 * "nordvpn connect" is done asynchronously. Hence only the #ServerController
 * will get the #ProcessResult, but cannot call the Mediator to tell the UI to
 * update. This would be againts the constraints of the 3/n-tier architecture
 * used in this application (see @ref architecture). However, The UI
 * update gets triggered by a separate action sequence that is -- from the
 * programs points of view -- independent of the first one.
 *
 * **Connection status updates**
 *
 * 1. At app start: The #ConnectionMediator creates receives a reference to
 * #ServerController, attaches to it and starts its background task.
 * 2. The #ServerController periodically calls "nordvpn status" to update the
 * information about the connection status.
 * 3. #ServerController notifies all subsribers implementing
 * #IConnectionInfoSubscription inclduing the #ConnectionMediator.
 * 4. #ConnectionMediator updates all its properties according to the
 * #ConnectionInfo object it receives.
 * 5. The Qt property system (https://doc.qt.io/qt-5/properties.html) handles
 * the automatic update of all property bindings in the UI, e.g.
 * #ConnectionMediator::isConnected, #ConnectionMediator::isConnecting, etc.
 *
 * @startuml
 *
 * autoactivate on
 *
 * box ui #eef8ff
 *   participant ConnectionStatusPanel
 * end box
 * box logic #eefff8
 *   participant ConnectionMediator
 *   participant StatusController
 * end box
 * box common #ffeef8
 *   participant Process
 * end box
 * box "\texternal\t" #f8f8f8
 *  boundary nordvpn
 * end box
 * activate ConnectionMediator
 * note over ConnectionMediator
 * Start of application:
 * ConnectionMediator
 * gets instantiated
 * end note
 * ConnectionMediator ->> StatusController : attach()
 * deactivate StatusController
 * ConnectionMediator ->> StatusController : startBackgroundTask()
 * deactivate ConnectionMediator
 * StatusController ->> StatusController : _backgroundTask()
 * loop forever
 *   StatusController -> StatusController : getStatus()
 *   StatusController -> Process : execute("nordvpn status")
 *   Process -> nordvpn : /bin/sh -c "nordvpn status"
 *   nordvpn --> Process : stdout + stderr
 *   Process --> StatusController : ProcessResult
 *   StatusController --> StatusController : ConnectionInfo
 *   StatusController -> StatusController : _notifySubscribers()
 *   StatusController ->> ConnectionMediator : updateConnectionInfo(...)
 *   StatusController --> StatusController : void
 *   ConnectionMediator ->> ConnectionStatusPanel : updated Q_PROPERTY's
 *   note over StatusController : sleep(1s)
 * end
 *
 * @enduml
 *
 */
