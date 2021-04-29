# --------------------------------------------------------------------------------
# initialize project
# --------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.14)
project(nordvpn-gui
  VERSION 1.0.0 # see below to set "-<pre-release>+<build-meta>" according to semver
  DESCRIPTION "UNOFFICIAL Graphical User Interface (GUI) for the NordVPN Linux Client"
  HOMEPAGE_URL "https://petwu.github.io/nordvpn-gui"
  LANGUAGES CXX
)

# --------------------------------------------------------------------------------
# include modules
# --------------------------------------------------------------------------------

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(Git)
include(Conan)
include(Cloc)
include(CodeQualityTools)
include(GNUInstallDirs)

# --------------------------------------------------------------------------------
# application description (see src/app.hpp.in as well)
# --------------------------------------------------------------------------------
set(SEMVER_MAJOR "${PROJECT_VERSION_MAJOR}")
set(SEMVER_MINOR "${PROJECT_VERSION_MINOR}")
set(SEMVER_PATCH "${PROJECT_VERSION_PATCH}")
set(SEMVER_PRE_RELEASE "dev")
get_git_hash(SEMVER_BUILD_META)
set(SEMVER_FULL "${SEMVER_MAJOR}.${SEMVER_MINOR}.${SEMVER_PATCH}")
if(SEMVER_PRE_RELEASE)
  string(APPEND SEMVER_FULL "-${SEMVER_PRE_RELEASE}")
endif()
if(SEMVER_BUILD_META)
  string(APPEND SEMVER_FULL "+${SEMVER_BUILD_META}")
endif()
set(AUTHOR "Peter Würth")
set(GITHUB_USERNAME "petwu")
set(GITHUB_REPOSITORY_NAME "nordvpn-gui")
set(REPOSITORY_URL "https://github.com/petwu/nordvpn-gui")
set(REPOSITORY_RELEASES_URL "${REPOSITORY_URL}/releases")
set(PROJECT_WEBSITE_URL "https://petwu.github.io/nordvpn-gui")
set(DOCS_URL "${PROJECT_WEBSITE_URL}/docs")
set(DONATE_URL "${PROJECT_WEBSITE_URL}/docs/donate")
set(ISSUES_URL "${PROJECT_WEBSITE_URL}/docs/contribute")
set(ORGANIZATION "NordVPN GUI")
set(APPLICATION_NAME "NordVPN GUI (Unofficial)")
set(APPLICATION_DESCRIPTION "${PROJECT_DESCRIPTION}")
set(LICENSE "GPLv3")
file(READ "LICENSE" LICENSE_TEXT)
file(READ "CHANGELOG.md" CHANGELOG)
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
find_package(Qt5 COMPONENTS Core REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QML_IMPORT_PATH ${CMAKE_SOURCE_DIR}/res
  CACHE STRING "additional qml module paths for Qt Creator"
  FORCE
)

# --------------------------------------------------------------------------------
# install external dependencies (except Qt) with conan
# --------------------------------------------------------------------------------
conan_cmake_run(
  CONANFILE conanfile.txt
  BUILD missing
  BASIC_SETUP
  CMAKE_TARGETS
  OUTPUT_QUIET
)

# --------------------------------------------------------------------------------
# options
# --------------------------------------------------------------------------------
option(NORDVPN_ENABLE_UPDATE_CHECK
  "If this option is on, the resulting variable will automatically check for\
  on the GitHub releases page and notify the user if an update is available. \
  This option should be ON when building from source or for building a\
  downloadable deb/rpm file for the GitHub release page. It should be OFF when\
  building for software repositories, since the the package manager of the\
  distribution is reponsible for updates."
  ON
)

# --------------------------------------------------------------------------------
# main executable
# --------------------------------------------------------------------------------
set(RESOURCES_DIR_NAME ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME})
add_subdirectory(src)
add_subdirectory(res)
add_subdirectory(doc)

# --------------------------------------------------------------------------------
# create utility targets
# --------------------------------------------------------------------------------

add_clang_format(clang-format FILES src/**/*.cpp src/**/*.hpp)
add_clang_tidy(clang-tidy TARGETS nordvpn-gui nordvpn)
add_iwyu(iwyu MAPPING_FILE .iwyu.imp)
add_cloc(cloc SOURCES cmake doc res/Style res/CMakeLists.txt src)

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
