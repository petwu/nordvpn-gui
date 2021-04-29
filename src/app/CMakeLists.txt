# --------------------------------------------------------------------------------
# configure application with defines in *.hpp.in files
# --------------------------------------------------------------------------------
configure_file(src/app.hpp.in src/app.hpp @ONLY)

# --------------------------------------------------------------------------------
# main executable
# --------------------------------------------------------------------------------
find_package(Qt5 COMPONENTS Core Quick Widgets Svg REQUIRED)
find_package(Threads REQUIRED)
add_executable(nordvpn-gui)
target_link_libraries(nordvpn-gui PRIVATE
  Qt5::Core Qt5::Quick Qt5::Widgets Qt5::Svg
  Threads::Threads
  lib::nordvpn
  ${CONAN_TARGETS}
)
target_include_directories(nordvpn-gui
  PRIVATE src "${CMAKE_CURRENT_BINARY_DIR}/src"
)

include(GNUInstallDirs)
install(TARGETS nordvpn-gui)

# --------------------------------------------------------------------------------
# compile controls a.k.a. preprocessor definitions
# --------------------------------------------------------------------------------
target_compile_definitions(nordvpn-gui PRIVATE
  IS_DEBUG=$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>
  NORDVPN_ENABLE_UPDATE_CHECK=$<BOOL:${NORDVPN_ENABLE_UPDATE_CHECK}>
)

# --------------------------------------------------------------------------------
# source files
# --------------------------------------------------------------------------------
target_sources(nordvpn-gui PRIVATE
  # Qt resources
  qml.qrc
  # main
  src/main.cpp
  src/runguard.cpp
  # mediators
  src/mediators/accountmediator.cpp
  src/mediators/connectionmediator.cpp
  src/mediators/devmediator.cpp
  src/mediators/mapimageprovider.cpp
  src/mediators/navmediator.cpp
  src/mediators/preferencesmediator.cpp
  src/mediators/qmldataconverter.cpp
  src/mediators/recentsmediator.cpp
  src/mediators/traymediator.cpp
  src/mediators/updatemediator.cpp
)
