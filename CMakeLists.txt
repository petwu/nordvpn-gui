# --------------------------------------------------------------------------------
# initialize project
# --------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.14)
project(nordvpn-gui
  VERSION 0.0.0
  DESCRIPTION "UNOFFICIAL Graphical User Interface (GUI) for the NordVPN Linux Client"
  HOMEPAGE_URL "https://petwu.github.io/nordvpn-gui"
  LANGUAGES CXX
)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# --------------------------------------------------------------------------------
# application description (see src/app.h.in as well)
# --------------------------------------------------------------------------------
set(AUTHOR "Peter Würth")
set(REPOSITORY_URL "https://github.com/petwu/nordvpn-gui")
set(PROJECT_WEBSITE_URL "https://petwu.github.io/nordvpn-gui")
set(DOCS_URL "${PROJECT_WEBSITE_URL}/docs")
set(DONATE_URL "${PROJECT_WEBSITE_URL}/docs/donate")
set(ISSUES_URL "${PROJECT_WEBSITE_URL}/docs/contribute")
set(ORGANIZATION "NordVPN GUI")
set(APPLICATION_NAME "NordVPN GUI (Unofficial)")
set(APPLICATION_DESCRIPTION "${PROJECT_DESCRIPTION}")
set(LICENSE "GPLv3")
file(READ "LICENSE" LICENSE_TEXT)
set(NORDVPN_DOWNLOAD_LINUX_URL "https://nordvpn.com/download/linux")
set(NORD_ACCOUNT_URL "https://my.nordaccount.com")

# --------------------------------------------------------------------------------
# C++ settings
# --------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --------------------------------------------------------------------------------
# Qt settings
# --------------------------------------------------------------------------------
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QML_IMPORT_PATH ${CMAKE_SOURCE_DIR}/res
  CACHE STRING "additional qml module paths for Qt Creator"
  FORCE
)

# --------------------------------------------------------------------------------
# install external dependencies (except Qt) with conan
# --------------------------------------------------------------------------------
include(Conan)
conan_cmake_run(
  CONANFILE conanfile.txt
  BUILD missing
  BASIC_SETUP
  CMAKE_TARGETS
  OUTPUT_QUIET
)

# --------------------------------------------------------------------------------
# main executable
# --------------------------------------------------------------------------------
find_package(Qt5 COMPONENTS Core Quick Widgets Svg REQUIRED)
find_package(Threads REQUIRED)
add_executable(${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE
  Qt5::Core Qt5::Quick Qt5::Widgets Qt5::Svg
  Threads::Threads
  ${CONAN_TARGETS}
)
include(GNUInstallDirs)
set(RESOURCES_DIR_NAME ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME})
add_subdirectory(src)
add_subdirectory(res)
add_subdirectory(doc)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME})

# --------------------------------------------------------------------------------
# compile controls a.k.a. preprocessor definitions
# --------------------------------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  target_compile_definitions(${PROJECT_NAME} PRIVATE IS_DEBUG=1)
endif()

# --------------------------------------------------------------------------------
# create targets for clang-tidy and clang-format
# --------------------------------------------------------------------------------

include(CodeQualityTools)
add_clang_tidy(clang-tidy TARGET ${PROJECT_NAME})
add_clang_format(clang-format FILES src/**/*.cpp src/**/*.h)
add_iwyu(iwyu MAPPING_FILE .iwyu.imp)

include(Cloc)
add_cloc(cloc SOURCES
  cmake
  doc
  res/Style
  res/CMakeLists.txt
  src
)

# --------------------------------------------------------------------------------
# configure CPack to create installable packages
# --------------------------------------------------------------------------------

# create tar.xz archive with source files
set(CPACK_SOURCE_GENERATOR TXZ)
# create .deb and .rpm packages for Debian and 
set(CPACK_GENERATOR TXZ DEB RPM)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_CONTACT ${AUTHOR})
set(CPACK_PACKAGE_VENDOR ${ORGANIZATION})
set(CPACK_PACKAGE_ICON ${CMAKE_CURRENT_LIST_DIR}/res/icons/icon.ico)
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_LIST_DIR}/LICENSE)
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_LIST_DIR}/README.md)
set(CPACK_VERBATIM_VARIABLES ON)
set(CPACK_SOURCE_IGNORE_FILES "/\\.git/" "/build/")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_SECTION "web")
set(CPACK_RPM_FILE_NAME RPM-DEFAULT)
set(CPACK_RPM_PACKAGE_LICENSE ${LICENSE})
set(CPACK_OUTPUT_FILE_PREFIX "packages")

include(CPack)
