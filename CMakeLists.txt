cmake_minimum_required(VERSION 3.12)
project(nordvpn-gui LANGUAGES CXX)

# --------------------------------------------------------------------------------
# application description
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

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# create compile_commands.json aka. compilation database
# (only make and ninja generators)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --------------------------------------------------------------------------------
# Qt settings
# --------------------------------------------------------------------------------

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QML_IMPORT_PATH ${CMAKE_SOURCE_DIR}/res
    CACHE STRING "additional qml module paths for Qt Creator"
    FORCE
)

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

find_package(Qt5 COMPONENTS Core Quick Widgets LinguistTools REQUIRED)
find_package(Threads REQUIRED)
find_package(Doxygen)

# --------------------------------------------------------------------------------
# configure application with defines in app.h.in
# --------------------------------------------------------------------------------

configure_file(src/app.h.in app.h)

# --------------------------------------------------------------------------------
# copy resources that are not compiled in by rcc (qrc)
# --------------------------------------------------------------------------------

file(COPY res/data/ DESTINATION res/data/)

# --------------------------------------------------------------------------------
# main executable
# --------------------------------------------------------------------------------

# source files
set(SRC_FILES
    src/main.cpp
    src/runguard.cpp
    # common
    src/common/io/asyncprocess.cpp
    src/common/io/process.cpp
    src/common/io/processresult.cpp
    src/common/util/strings.cpp
    # data
    src/data/enums/group.cpp
    src/data/enums/protocol.cpp
    src/data/enums/technology.cpp
    src/data/enums/securityprotocol.cpp
    src/data/models/connectable.cpp
    src/data/models/country.cpp
    src/data/models/location.cpp
    src/data/models/server.cpp
    src/data/repositories/baserepository.cpp
    src/data/repositories/preferencesrepository.cpp
    src/data/repositories/serverrepository.cpp
    # logic
    src/logic/mediators/accountmediator.cpp
    src/logic/mediators/connectionmediator.cpp
    src/logic/mediators/navmediator.cpp
    src/logic/mediators/preferencesmediator.cpp
    src/logic/mediators/qmldataconverter.cpp
    src/logic/mediators/recentsmediator.cpp
    src/logic/mediators/traymediator.cpp
    src/logic/models/connectioninfo.cpp
    src/logic/models/envinfo.cpp
    src/logic/models/nordvpnsettings.cpp
    src/logic/models/whitelistportentry.cpp
    src/logic/nordvpn/accountcontroller.cpp
    src/logic/nordvpn/basecontroller.cpp
    src/logic/nordvpn/envcontroller.cpp
    src/logic/nordvpn/servercontroller.cpp
    src/logic/nordvpn/preferencescontroller.cpp
    src/logic/nordvpn/statuscontroller.cpp
)

# add translations
set(TS_FILES
    res/translations/en.ts
    res/translations/de.ts
)
set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION res/translations)
qt5_add_translation(QM_FILES ${TS_FILES})
configure_file(res/translations/translations.qrc.in res/translations/translations.qrc COPYONLY)
add_custom_target(lupdate
    COMMAND ${Qt5_LUPDATE_EXECUTABLE} -source-language en_US -no-obsolete -recursive app -ts ${TS_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Updating translations res/translations/*.ts"
)

# add resource files
qt5_add_resources(SRC_FILES
    src/qml.qrc
    res/res.qrc
    ${CMAKE_BINARY_DIR}/res/translations/translations.qrc
)

# main target
add_executable(nordvpn-gui ${SRC_FILES} ${QM_FILES})
target_include_directories(nordvpn-gui PRIVATE
    src
    ${CMAKE_BINARY_DIR}
)
target_link_libraries(nordvpn-gui PRIVATE
    Qt5::Core Qt5::Quick Qt5::Widgets
    Threads::Threads
    ${CONAN_TARGETS}
)

# --------------------------------------------------------------------------------
# generate documentation with doxygen
# --------------------------------------------------------------------------------

if(Doxygen_FOUND)

    configure_file(docs/doxyfile.in doxyfile @ONLY)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
    )

else()

    add_custom_target(docs
        COMMAND ${CMAKE_COMMAND} -E echo "Error: Doxygen not found"
    )

endif()
