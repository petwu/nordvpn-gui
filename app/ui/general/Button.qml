import QtQuick 2.12
import QtQuick.Controls 2.12

Button {
    focusPolicy: Qt.TabFocus
    Keys.onEnterPressed: clicked()
    Keys.onReturnPressed: clicked()
}
