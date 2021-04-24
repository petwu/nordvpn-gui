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
        category: 'MainWindow'
        property alias x: window.x
        property alias y: window.y
        property alias width: window.width
        property alias height: window.height
    }

    Connections {
        target: TrayMediator
        onToggleMainWindowAction: {
            if (window.visible){
                window.hide()
            } else {
                window.show()
                window.raise()
                window.requestActivate()
            }
        }
        onShowMainWindowAction: {
            window.show()
            window.raise()
            window.requestActivate()
        }
    }

    Connections {
        target: NavMediator
        onMainWindowViewSourceChanged: setView()
    }

    Component.onCompleted: setView()

    function setView() {
        /*
         * For some reason (don't know why) displaying map.svg as part of MainWindow.qml
         * causes the Loader to hang and not respond any more -- but only if MainWindow.qml
         * gets loaded by settings the Loaders source property. If the source property is set
         * to MainWindow.qml from the start, everything works as expected.
         * Hence this little workaround: MainWindow.qml gets loaded with a seperated Loader
         * and toggled visible if needed. The other Loader handles every other view.
         */
        const qmlFile = NavMediator.mainWindowViewSource
        if (qmlFile.includes('MainView')) {
            mainLoader.visible = true
            otherLoader.source = ''
        } else {
            mainLoader.visible = false
            if (otherLoader.source !== qmlFile) {
                otherLoader.source = qmlFile
            }
        }
    }

    Loader {
        id: mainLoader
        anchors.fill: parent
        visible: false
        source: 'qrc:/ui/views/MainView.qml'
    }

    Loader {
        id: otherLoader
        anchors.fill: parent
        asynchronous: true
        visible: !mainLoader.visible
    }
}
