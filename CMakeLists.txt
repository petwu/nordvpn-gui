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

configure_file(app/app.h.in app.h)

# --------------------------------------------------------------------------------
# copy resources that are not compiled in by rcc (qrc)
# --------------------------------------------------------------------------------

file(COPY res/data/ DESTINATION res/data/)

# --------------------------------------------------------------------------------
# main executable
# --------------------------------------------------------------------------------

# source files
set(SRC_FILES
    app/main.cpp
    app/runguard.cpp
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
    app/logic/mediators/accountmediator.cpp
    app/logic/mediators/mediator.cpp
    app/logic/mediators/navmediator.cpp
    app/logic/mediators/preferencesmediator.cpp
    app/logic/mediators/qmldataconverter.cpp
    app/logic/mediators/traymediator.cpp
    app/logic/models/connectioninfo.cpp
    app/logic/models/envinfo.cpp
    app/logic/models/nordvpnsettings.cpp
    app/logic/models/whitelistportentry.cpp
    app/logic/nordvpn/accountcontroller.cpp
    app/logic/nordvpn/basecontroller.cpp
    app/logic/nordvpn/envcontroller.cpp
    app/logic/nordvpn/servercontroller.cpp
    app/logic/nordvpn/preferencescontroller.cpp
    app/logic/nordvpn/statuscontroller.cpp
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
    app/qml.qrc
    res/res.qrc
    ${CMAKE_BINARY_DIR}/res/translations/translations.qrc
)

# main target
add_executable(nordvpn-gui ${SRC_FILES} ${QM_FILES})
target_include_directories(nordvpn-gui PUBLIC
    app
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
