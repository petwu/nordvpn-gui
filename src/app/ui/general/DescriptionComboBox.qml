import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import Style 1.0

Column {
    id: descComboBox
    width: parent.width
    spacing: 6

    property string label: ''
    property string description: ''
    property var model: []
    property string defaultText: ''
    readonly property alias currentText: comboBox.currentText

    signal activated(int index)

    Component.onCompleted: {
        if (descComboBox.defaultText !== '') {
            const index = comboBox.find(descComboBox.defaultText, Qt.MatchExactly)
            if (index >= 0) {
                comboBox.currentIndex = index
            }
        }
    }

    RowLayout {
        width: parent.width
        spacing: 16

        Label {
            id: text
            text: descComboBox.label
            wrapMode: Label.Wrap
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true
        }

        ComboBox {
            id: comboBox
            model: descComboBox.model
            editable: false
            focusPolicy: Qt.TabFocus
            onActivated: (i) => descComboBox.activated(i)
        }
    }

    Label {
        text: descComboBox.description
        font.pointSize: .85 * text.font.pointSize
        color: Style.colorTextAlternate
        wrapMode: Label.Wrap
        visible: descComboBox.description.length > 0
        width: parent.width
        horizontalAlignment: Text.AlignJustify
    }
}
