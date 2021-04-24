import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import Style 1.0

Column {
    id: descCheckBox
    width: parent.width
    Layout.fillWidth: true
    spacing: 6

    property string label: ''
    property string description: ''
    property alias checked: checkBox.checked
    readonly property double descriptionIndentation: checkBox.indicator.width + checkBox.padding

    CheckBox {
        id: checkBox
        text: descCheckBox.label
    }

    Label {
        text: descCheckBox.description
        x: descCheckBox.descriptionIndentation
        width: parent.width - x
        font.pointSize: .85 * checkBox.font.pointSize
        color: Style.colorTextAlternate
        wrapMode: Label.Wrap
        visible: descCheckBox.description.length > 0
    }
}
