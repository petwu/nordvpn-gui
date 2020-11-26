import QtQuick 2.12

import Style 1.0

Item {
    width: size
    height: size

    property int size: 20
    property double lineWidth: Math.max(1, width/16)
    property color color: Style.colorIcon
}
