import QtQuick 2.12
import QtQuick.Controls 2.12

import '.'
import '../general'

ScrollView {
    Column {
        width: parent.width

        Collapsible {
            title: 'Recents'

            Repeater {
                model: SidePanelMediator.recentCountries

                Text {
                    text: modelData.name
                    padding: 8
                }
            }
        }

        Collapsible {
            title: 'Specialty Servers'

            Rectangle {
                width: parent.width
                height: 300
                color: '#ffbbbb'
            }
        }
    }
}
