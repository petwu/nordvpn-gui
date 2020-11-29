# --------------------------------------------------------------------------------
# configure application with defines in app.h.in
# --------------------------------------------------------------------------------
configure_file(app.h.in app.h)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# --------------------------------------------------------------------------------
# source files
# --------------------------------------------------------------------------------
target_sources(${PROJECT_NAME} PRIVATE
  main.cpp
  runguard.cpp
  qml.qrc
  # common
  common/io/asyncprocess.cpp
  common/io/process.cpp
  common/io/processresult.cpp
  common/util/strings.cpp
  # data
  data/enums/group.cpp
  data/enums/protocol.cpp
  data/enums/technology.cpp
  data/enums/securityprotocol.cpp
  data/models/connectable.cpp
  data/models/country.cpp
  data/models/location.cpp
  data/models/server.cpp
  data/repositories/baserepository.cpp
  data/repositories/preferencesrepository.cpp
  data/repositories/serverrepository.cpp
  # logic
  logic/mediators/accountmediator.cpp
  logic/mediators/connectionmediator.cpp
  logic/mediators/navmediator.cpp
  logic/mediators/preferencesmediator.cpp
  logic/mediators/qmldataconverter.cpp
  logic/mediators/recentsmediator.cpp
  logic/mediators/traymediator.cpp
  logic/models/connectioninfo.cpp
  logic/models/envinfo.cpp
  logic/models/nordvpnsettings.cpp
  logic/models/whitelistportentry.cpp
  logic/nordvpn/accountcontroller.cpp
  logic/nordvpn/basecontroller.cpp
  logic/nordvpn/envcontroller.cpp
  logic/nordvpn/servercontroller.cpp
  logic/nordvpn/preferencescontroller.cpp
  logic/nordvpn/statuscontroller.cpp
)
