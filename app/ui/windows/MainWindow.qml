import QtQuick 2.12
import QtQuick.Controls 2.12
import Qt.labs.settings 1.1

import '../views'

ApplicationWindow {
    id: window
    visible: true
    title: ApplicationName
    minimumWidth: 720
    minimumHeight: 400
    width: 1000
    height: 600

    Settings {
        property alias x: window.x
        property alias y: window.y
        property alias width: window.width
        property alias height: window.height
    }

    Loader {
        anchors.fill: parent
        source: NavMediator.viewSource
    }
}
