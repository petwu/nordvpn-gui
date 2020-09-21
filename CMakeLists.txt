cmake_minimum_required(VERSION 3.12)

project(nordvpn-gui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Quick LinguistTools REQUIRED)

set(TS_FILES locale/en_US.ts)

add_executable(nordvpn-gui
    # resource files
    app/qml.qrc
    res/res.qrc
    # translations
    ${TS_FILES}
    # source files
    app/main.cpp
    app/logic/nordvpn/basecontroller.cpp
    app/logic/nordvpn/statuscontroller.cpp
    app/logic/mediators/mapmediator.cpp
)

target_include_directories(nordvpn-gui PUBLIC app/)

target_link_libraries(nordvpn-gui PRIVATE Qt5::Core Qt5::Quick)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
