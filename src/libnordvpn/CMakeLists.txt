# --------------------------------------------------------------------------------
# configure application with defines in *.hpp.in files
# --------------------------------------------------------------------------------
include(GNUInstallDirs)
file(RELATIVE_PATH REL_PATH_TO_RESOURCES
  "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}"
  "${CMAKE_INSTALL_PREFIX}/${RESOURCES_DIR_NAME}"
)
configure_file(src/libnordvpn.hpp.in src/libnordvpn.hpp @ONLY)
configure_file(include/nordvpn/config.hpp.in include/nordvpn/config.hpp @ONLY)

# --------------------------------------------------------------------------------
# libnordvpn
# --------------------------------------------------------------------------------
add_library(nordvpn)
add_library(lib::nordvpn ALIAS nordvpn)
target_link_libraries(nordvpn PRIVATE ${CONAN_TARGETS})
target_include_directories(nordvpn
  PRIVATE src "${CMAKE_CURRENT_BINARY_DIR}/src"
  PUBLIC include "${CMAKE_CURRENT_BINARY_DIR}/include"
)

# --------------------------------------------------------------------------------
# source files
# --------------------------------------------------------------------------------
target_sources(nordvpn PRIVATE
  src/factory.cpp
  # enums
  src/enums/connectionstatus.cpp
  src/enums/group.cpp
  src/enums/protocol.cpp
  src/enums/technology.cpp
  # io
  src/io/asyncprocess.cpp
  src/io/process.cpp
  src/io/processresult.cpp
  # models
  src/models/connectable.cpp
  src/models/connectioninfo.cpp
  src/models/country.cpp
  src/models/envinfo.cpp
  src/models/location.cpp
  src/models/nordvpnsettings.cpp
  src/models/server.cpp
  src/models/version.cpp
  src/models/whitelistportentry.cpp
  # util
  src/util/strings.cpp
  # controllers
  src/controllers/accountcontroller.cpp
  src/controllers/basecontroller.cpp
  src/controllers/connectioncontroller.cpp
  src/controllers/countrycontroller.cpp
  src/controllers/envcontroller.cpp
  src/controllers/preferencescontroller.cpp
  src/controllers/recentscontroller.cpp
  src/controllers/servercontroller.cpp
  src/controllers/statuscontroller.cpp
  # repositories
  src/repositories/baserepository.cpp
  src/repositories/preferencesrepository.cpp
  src/repositories/serverrepository.cpp
  src/repositories/updatecheckrepository.cpp
)
