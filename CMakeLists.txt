cmake_minimum_required(VERSION 3.12)
project(nordvpn-gui LANGUAGES CXX)

# --------------------------------------------------------------------------------
# application description
# --------------------------------------------------------------------------------

set(AUTHOR "Peter Würth")
set(REPOSITORY "https://github.com/petwu/nordvpn-gui")
set(ORGANIZATION "NordVPN GUI")
set(APPLICATION_NAME "NordVPN GUI")
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

# --------------------------------------------------------------------------------
# C++ settings
# --------------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------------------------------------------------------------
# Qt settings
# --------------------------------------------------------------------------------

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QML_IMPORT_PATH ${CMAKE_SOURCE_DIR}/res
    CACHE STRING "additional qml module paths for Qt Creator"
    FORCE)

# --------------------------------------------------------------------------------
# install external dependencies (except Qt) with conan
# => https://github.com/conan-io/cmake-conan
# --------------------------------------------------------------------------------

set(CONAN_CMAKE_URL
    "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake")
if(NOT EXISTS ${CMAKE_BINARY_DIR}/conan.cmake)
    file(DOWNLOAD ${CONAN_CMAKE_URL} ${CMAKE_BINARY_DIR}/conan.cmake)
endif()
include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_run(CONANFILE conanfile.txt BUILD missing BASIC_SETUP CMAKE_TARGETS)

# --------------------------------------------------------------------------------
# find required packages
# --------------------------------------------------------------------------------

find_package(Qt5 5.12 COMPONENTS Core Quick Widgets LinguistTools REQUIRED)
find_package(Threads REQUIRED)

# --------------------------------------------------------------------------------
# copy resources directory
# --------------------------------------------------------------------------------

file(COPY ${CMAKE_SOURCE_DIR}/res/ DESTINATION ${CMAKE_BINARY_DIR}/res/)

# --------------------------------------------------------------------------------
# configure application with defines in app.h.in
# --------------------------------------------------------------------------------

configure_file(app/app.h.in app.h)

# --------------------------------------------------------------------------------
# main executable
# --------------------------------------------------------------------------------

# source files
set(SRC_FILES
    app/main.cpp
    # common
    app/common/io/asyncprocess.cpp
    app/common/io/process.cpp
    app/common/io/processresult.cpp
    app/common/util/strings.cpp
    # data
    app/data/enums/group.cpp
    app/data/enums/protocol.cpp
    app/data/enums/technology.cpp
    app/data/enums/securityprotocol.cpp
    app/data/models/connectable.cpp
    app/data/models/country.cpp
    app/data/models/location.cpp
    app/data/models/server.cpp
    app/data/repositories/baserepository.cpp
    app/data/repositories/preferencesrepository.cpp
    app/data/repositories/serverrepository.cpp
    # logic
    app/logic/mediators/mediator.cpp
    app/logic/mediators/qmldataconverter.cpp
    app/logic/models/nordvpnsettings.cpp
    app/logic/nordvpn/basecontroller.cpp
    app/logic/nordvpn/servercontroller.cpp
    app/logic/nordvpn/settingscontroller.cpp
    app/logic/nordvpn/statuscontroller.cpp
)

# resource files
set(QRC_FILES
    app/qml.qrc
    res/res.qrc
)

# translations
set(TS_FILES
    locale/en_US.ts
)

# main target
add_executable(nordvpn-gui ${SRC_FILES} ${QRC_FILES} ${TS_FILES})
target_include_directories(nordvpn-gui PUBLIC ${CMAKE_BINARY_DIR})
target_include_directories(nordvpn-gui PUBLIC app/)
target_link_libraries(nordvpn-gui PRIVATE Qt5::Core Qt5::Quick Qt5::Widgets Threads::Threads ${CONAN_TARGETS})
qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
