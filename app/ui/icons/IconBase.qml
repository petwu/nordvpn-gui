import QtQuick 2.12

import Style 1.0

Item {
    width: 24
    height: width

    property int contentWidth: .75 * width
    property double lineWidth: Math.max(1, width/16)
    property color color: Style.colorIcon
}
