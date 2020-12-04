# --------------------------------------------------------------------------------
# initialize project
# --------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.13)
project(nordvpn-gui LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# --------------------------------------------------------------------------------
# application description (see src/app.h.in as well)
# --------------------------------------------------------------------------------
set(AUTHOR "Peter Würth")
set(REPOSITORY "https://github.com/petwu/nordvpn-gui")
set(ISSUES_BOARD "https://github.com/petwu/nordvpn-gui/issues")
set(ORGANIZATION "NordVPN GUI")
set(APPLICATION_NAME "NordVPN GUI")
set(APPLICATION_DESCRIPTION "Graphical User Interface (GUI) for the NordVPN Linux Client")
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
set(LICENSE "GPLv3")
file(READ LICENSE LICENSE_TEXT)
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
find_package(Qt5 COMPONENTS Core Quick Widgets LinguistTools REQUIRED)
find_package(Threads REQUIRED)
add_executable(${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE
  Qt5::Core Qt5::Quick Qt5::Widgets
  Threads::Threads
  ${CONAN_TARGETS}
)
add_subdirectory(src)
add_subdirectory(res)
add_subdirectory(doc)

# --------------------------------------------------------------------------------
# create targets for clang-tidy and clang-format
# --------------------------------------------------------------------------------

include(CodeQualityTools)
add_clang_tidy(clang-tidy TARGET ${PROJECT_NAME})
add_clang_format(clang-format FILES src/**/*.cpp src/**/*.h)
add_iwyu(iwyu MAPPING_FILE .iwyu.imp)
