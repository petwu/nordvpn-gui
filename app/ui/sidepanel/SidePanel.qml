import QtQuick 2.12
import QtQuick.Controls 2.12

import Style 1.0

import '.'
import '../general'
import '../icons'

ScrollView {
    id: scrollView
    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

    property int rowHeigth: 40
    property int iconTextSpacing: 8
    property int iconSize: 20
    property int minContentWidth: 100

    onAvailableWidthChanged: {
        if (availableWidth > minContentWidth) {
            sidePanel.width = availableWidth
        }
    }

    Column {
        id: sidePanel
        width: minContentWidth

        Collapsible {
            id: sectionRecents
            title: 'Recents'
            visible: Mediator.recentCountries.length > 0
            spacing: 0

            Repeater {
                model: Mediator.recentCountries

                RecentConnection {
                    width: sidePanel.width
                    height: rowHeigth
                    iconHorizontalCenter: sectionRecents.headerPadding+sectionRecents.handleSize/2
                    countryId: modelData.id
                    countryName: modelData.name
                }
            }
        }

        Collapsible {
            title: 'Specialty Servers'
        }

        Collapsible {
            id: sectionCountries
            title: 'Countries'
            spacing: 0

            Repeater {
                model: Mediator.countryList

                CountryItem {
                    width: sidePanel.width
                    height: rowHeigth
                    iconHorizontalCenter: sectionCountries.headerPadding+sectionCountries.handleSize/2
                    country: modelData
                    onPopupOpened: scrollView.enabled = false
                    onPopupClosed: scrollView.enabled = true
                }
            }
        }
    }
}
