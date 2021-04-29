import QtQuick 2.12

import '../general'

import 'qrc:/fonts/fontawesome5-solid.js' as Solid
import 'qrc:/fonts/fontawesome5-brands.js' as Brands

Text {
    text: iconNameToUnicode()
    font.family: fontFamily()
    font.pixelSize: size * Qt.application.font.pixelSize

    enum Type {
        Solid,
        Brands
    }

    property int type: Fa5Icon.Type.Solid
    property string iconName: 'icons'
    property double size: 1

    function iconNameToUnicode() {
        let unicode = Solid.Icons[iconName]
        if (unicode === undefined) {
            unicode = '\uf188' // bug
        }
        return unicode
    }

    function fontFamily() {
        switch (type) {
        case Fa5Icon.Type.Solid:
            return Fa5SolidFontFamily
        case Fa5Icon.Type.Brands:
            return Fa5BrandsFontFamily
        default:
            return Fa5SolidFontFamily
        }
    }
}
