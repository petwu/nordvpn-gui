import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import Style 1.0

import '../general'

ApplicationWindow {
    id: licenseWindow
    visible: true
    //: Title of the license window.
    title: qsTr('License') + ' — ' + ApplicationName
    width: scrollView.width
    height: 480
    minimumWidth: width
    minimumHeight: height
    maximumWidth: width
    maximumHeight: height
    flags: Qt.Dialog
    modality: Qt.ApplicationModal

    ScrollView {
        id: scrollView
        width: licenseText.width + ScrollBar.vertical.width
        height: parent.height
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        TextEdit {
            id: licenseText
            text: LicenseText
            font.family: MonospaceFontFamily
            padding: 16
            readOnly: true
            selectByMouse: true
            color: Style.colorText
        }
    }
}
