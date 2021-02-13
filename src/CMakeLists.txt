# --------------------------------------------------------------------------------
# configure application with defines in *.h.in files
# --------------------------------------------------------------------------------
configure_file(app.h.in app.h @ONLY)
include(GNUInstallDirs)
file(RELATIVE_PATH REL_PATH_TO_RESOURCES
  "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}"
  "${CMAKE_INSTALL_PREFIX}/${RESOURCES_DIR_NAME}"
)
configure_file(config.h.in config.h @ONLY)
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
  common/types/version.cpp
  common/util/strings.cpp
  # data
  data/enums/group.cpp
  data/enums/protocol.cpp
  data/enums/securityprotocol.cpp
  data/enums/technology.cpp
  data/models/connectable.cpp
  data/models/country.cpp
  data/models/location.cpp
  data/models/server.cpp
  data/repositories/baserepository.cpp
  data/repositories/preferencesrepository.cpp
  data/repositories/serverrepository.cpp
  data/repositories/updatecheckrepository.cpp
  # logic
  logic/enums/connectionstatus.cpp
  logic/mediators/accountmediator.cpp
  logic/mediators/connectionmediator.cpp
  logic/mediators/devmediator.cpp
  logic/mediators/mapimageprovider.cpp
  logic/mediators/navmediator.cpp
  logic/mediators/preferencesmediator.cpp
  logic/mediators/qmldataconverter.cpp
  logic/mediators/recentsmediator.cpp
  logic/mediators/traymediator.cpp
  logic/mediators/updatemediator.cpp
  logic/models/connectioninfo.cpp
  logic/models/envinfo.cpp
  logic/models/nordvpnsettings.cpp
  logic/models/whitelistportentry.cpp
  logic/nordvpn/accountcontroller.cpp
  logic/nordvpn/basecontroller.cpp
  logic/nordvpn/envcontroller.cpp
  logic/nordvpn/preferencescontroller.cpp
  logic/nordvpn/servercontroller.cpp
  logic/nordvpn/statuscontroller.cpp
)
