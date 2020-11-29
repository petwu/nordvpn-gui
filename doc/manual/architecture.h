/**
 * @page architecture Architecture
 *
 * @brief This page describes the software architecture of the application from
 * a high level perspective.
 *
 * @details It does not go into details about any specific classes, but
 * rather describes the different types of classes and their responsibilities as
 * well as fundamental design decisions.
 *
 * This application follows a basic 3-tier architecture. Each tier/layer has an
 * appropriate subdirectory in `src/`:
 *
 * @startuml "3-tier architecture"
 *
 * package src {
 *   package ui {
 *   }
 *   package logic {
 *   }
 *   package data {
 *   }
 *   package common {
 *   }
 *
 *   note left of ui
 *   tier 1: <b>UI Layer</b>
 *   a.k.a.: Presentation Layer
 *   end note
 *   note left of logic
 *   tier 2: <b>Logic Layer</b>
 *   a.k.a.: Application Layer, Service Layer,
 *               Business Logic Layer
 *   end note
 *   note left of data
 *   tier 3: <b>Data Layer</b>
 *   a.k.a.: Data Access Layer, Persistence Layer
 *   end note
 *   note right of common
 *   tier 0: <b>Common Layer</b>
 *   a.k.a. Cross-Cutting Layer
 *   This Layer is vertical while all others
 *   are horizontal.
 *   end note
 * }
 *
 * ui --> logic
 * logic --> data
 * logic -[norank]> common
 * data -> common
 *
 * @enduml
 *
 * The characteristics of each layers are discussed in the following sections.
 *
 * @section architecture_layers_ui UI Layer
 *
 * This layer contains files describing the applications user interface (UI).
 * This is done using QML (https://doc.qt.io/qt-5/qmlapplications.html), a
 * declarative markup language by Qt.
 *
 * @startuml "UI layer"
 *
 * package "src/ui" as ui {
 *   package windows {
 *   }
 *   package views {
 *   }
 *   package general {
 *   }
 *   package "<i><others></i>" as others {
 *   }
 * }
 *
 * package "src/logic" as logic {
 *   package mediators {
 *   }
 * }
 *
 * windows -[hidden]-> views
 * windows --> general
 * views --> general
 * general <-- others
 * ui ------> mediators
 *
 * note right of windows
 * Contains QML files whose
 * top-level element is a <i>Window</i>
 * or <i>ApplicationWindow</i>.
 * end note
 *
 * note right of views
 * Views that
 * are displayed
 * inside a window.
 * end note
 *
 * note right of general
 * Basic tiems or controls that
 * are useful in many other
 * places or override/extend
 * standard QML items/controls.
 * end note
 *
 * note right of others
 * Group items/controls that
 * are closely related, e.g.
 * all map related controls
 * like map markers.
 * end note
 *
 * @enduml
 *
 * **Responsibilities, Does and Don'ts**
 *
 * The UI layer is responsible for ...
 * - describing the user interface elements like buttons, lists, checkboxes etc.
 * - exchaning data and events with the logic layer
 * .
 * Do ...
 * - make windows either self-contained ("single view window") or split them up
 * into different views
 * - keep items/controls small and readable
 * - create reusable controls where prossible and appropriate
 * - work with translatable strings (`qsTr()` etc.) where necessary
 * .
 * Don't ...
 * - implement any business logic in the UI layer
 *   - some imperative part can be implemented using JavaScript, but they should
 * be kept to a minimum
 *   - knowledge about the current state of something should always be received
 * from the logic layer using property bindings
 * - have many hard coded styles (colors, fonts, font sizes, etc.), but import
 * the @ref resources_style where possible
 *
 * @section architecture_layers_logic Logic Layer
 *
 * This layer contains the business logic of the application.
 *
 * @startuml logic layer
 *
 * package "src/ui" as ui {
 * }
 *
 * package "src/logic" as logic {
 *   package enums {
 *   }
 *   package models {
 *   }
 *   package nordvpn {
 *   }
 *   package mediators {
 *   }
 * }
 *
 * package "src/data" as data {
 * }
 *
 * ui --> mediators
 * mediators --> nordvpn
 * mediators --> models
 * mediators --> enums
 * nordvpn --> models
 * nordvpn --> enums
 * nordvpn -[norank]-> data
 * models --> enums
 * models ---> data
 *
 * note right of mediators
 * ui <-> logic communication
 * end note
 *
 * note right of nordvpn
 * logic <-> data communication
 * logic <-> nordvpn CLI communication
 * end note
 *
 * note right of models
 * data classes
 * end note
 *
 * note right of enums
 * enums/enum classes and related functions
 * end note
 *
 * @enduml
 *
 * The logic layer is divided into multiple packages that contain classes of
 * different types and with different responsibilities:
 *
 * **mediators**
 * - Mediators are responsible for communicating with the UI layer.
 * - They provide bindable properties (via the `Q_PROPERTY` macro, see
 * https://doc.qt.io/qt-5/properties.html) that notify the UI about changes.
 * - They also provide _"slots"_ a.k.a. functions that are accessible inside QML
 * code so that the UI can send command to the logic layer.
 * - Any mediator has to extend the Qt class `QObject` in order to make use of
 * those features.
 * - **The mediators are the only place except the UI, where Qt code is
 * allowed!** Everywhere else, only standard C++ code shall be used.
 * - Mediators should not contain much logic, but simply provided data and
 * functions to the UI and delegate everything else to appropriate controllers.
 * - Mediators have to be added to the Qt application as context objects in
 * order to be accessible in the QML code:
 * @code
 * QQmlApplicationEngine engine;
 * QQmlContext *ctx = engine.rootContext();
 * ctx->setContextProperty("MyMediator", new MyMediator());
 * @endcode
 * Note that they are available globally this way. So it is reasonalbe to give
 * them distiguishable names and not call one only "Mediator", since this would
 * make it harder to find all places in the UI layer, where a specific mediator
 * is used.
 *
 * **nordvpn**
 * - This package contains controllers that implement the main business logic.
 * - Controllers call the _nordvpn_ command line tool e.g. to connect to or
 * disconnect from a VPN server.
 * - Controllers use repositories from the data layer to fetch additional data.
 *
 * **models** and **enums**
 * - Those who packages simply provided data classes e.g. for data provided by a
 * controller.
 * - Models may use models and enums from the data layer as class attributes.
 * - They should be kept simple.
 *
 * @section architecture_layers_data Data Layer
 *
 * The data layer allows reading and writing data from different sources like
 * the NordVPN API or the filesystem.
 *
 * @startuml data layer
 *
 * package "src/logic" as logic {
 * }
 *
 * package "src/data" as data {
 *   package repositories {
 *   }
 *   package models {
 *   }
 *   package enums {
 *   }
 * }
 *
 * cloud "NordVPN API" as internet {
 * }
 * cloud filesystem {
 * }
 * cloud "    ...    " as others {
 * }
 *
 * logic --> repositories
 * models -> enums
 * repositories --> models
 * repositories --> enums
 * repositories -> internet
 * repositories --> filesystem
 * repositories --> others
 *
 * @enduml
 *
 * The data layer is divided into different packages that contain classes of
 * different types with different responsibilities:
 *
 * **repositories**
 *
 * Repositories provided functions to access external ressources:
 * - NordVPN API: fetch additional informations about server that are not
 * accessible via the command line tool
 * - filesystem: read and save files to store information relevant to the
 * application
 * .
 * Repositories are not used to access the _nordvpn_ command line tool. This is
 * the responsibility of controllers in the logic layer.
 *
 * **models** and **enums**
 *
 * Those two packages provided data classes and enum values, e.g. to represent
 * servers or countries. They are used by the logic layer and therefore in most
 * parts of the application.
 *
 * @section architecture_layers_common Common Layer
 *
 * The common layer contains cross-cutting concerns
 * (https://en.wikipedia.org/wiki/Cross-cutting_concern), meaning that code that
 * is quite general and cannot be categorized into one of the other layers, but
 * should be usable by multuple layers. Things that fit into the common layer:
 * - additional helper functions to manupulate classes of the C++ Standard
 * Library, e.g. additional string manipulation functions
 * - logging
 * - ...
 *
 * **Important**:
 * - The common layer should not contains any business logic.
 * - Depedencies are allowed only one-way towards the common layer. The common
 * layer must not depend on anything from the other three layers.
 *
 * @note In case of this application the UI cannot depend on the common layer,
 * since the UI layer is written in QML while all other layer are written in
 * C++. But this is only for technical reasons and not because of architectural
 * decision.
 *
 */
