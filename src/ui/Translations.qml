import QtQuick 2.12

/*!
 * This file is not intended to be used by the application but only to be
 * found by lupdate (Qt Linguistic Tool) to generate translation files.
 * The translations then can be used globally with qsTranslate(<context>, <string>)
 * by providing the correct context.
 */

Item {
    Component.onCompleted: {
        console.log()
        console.log('--------------------------------------------------------------------------------')
        console.log('WARNING:                                                                        ')
        console.log('````````                                                                        ')
        console.log('You included translations.qml. This file is only intended to be used by lupdate ')
        console.log('to generate translation files (.ts). Why are you using this file?               ')
        console.log('--------------------------------------------------------------------------------')
        console.log()
    }

    // Translation of time units (the abbreviates versions).
    // Usage: qsTranslate('TimeUnit', <your time unit or variable here>)
    readonly property var timeunits: [
        QT_TRANSLATE_NOOP('TimeUnit', 's'),
        QT_TRANSLATE_NOOP('TimeUnit', 'min'),
        QT_TRANSLATE_NOOP('TimeUnit', 'h'),
        QT_TRANSLATE_NOOP('TimeUnit', 'd'),
        QT_TRANSLATE_NOOP('TimeUnit', 'yr'),
    ]

    // Translations of all country names used by NordVPN (as of 2020-11-08).
    // Usage: qsTranslate('Country', <your country name or variable here>)
    readonly property var countries: [
        QT_TRANSLATE_NOOP('Country', 'Albania'),
        QT_TRANSLATE_NOOP('Country', 'Argentina'),
        QT_TRANSLATE_NOOP('Country', 'Australia'),
        QT_TRANSLATE_NOOP('Country', 'Austria'),
        QT_TRANSLATE_NOOP('Country', 'Belgium'),
        QT_TRANSLATE_NOOP('Country', 'Bosnia and Herzegovina'), // <--| for some reason both spellings appear in the
        QT_TRANSLATE_NOOP('Country', 'Bosnia And Herzegovina'), // <--| JSON response of api.nordvpn.com/v1/servers?limit=-1
        QT_TRANSLATE_NOOP('Country', 'Brazil'),
        QT_TRANSLATE_NOOP('Country', 'Bulgaria'),
        QT_TRANSLATE_NOOP('Country', 'Canada'),
        QT_TRANSLATE_NOOP('Country', 'Chile'),
        QT_TRANSLATE_NOOP('Country', 'Costa Rica'),
        QT_TRANSLATE_NOOP('Country', 'Croatia'),
        QT_TRANSLATE_NOOP('Country', 'Cyprus'),
        QT_TRANSLATE_NOOP('Country', 'Czech Republic'),
        QT_TRANSLATE_NOOP('Country', 'Denmark'),
        QT_TRANSLATE_NOOP('Country', 'Estonia'),
        QT_TRANSLATE_NOOP('Country', 'Finland'),
        QT_TRANSLATE_NOOP('Country', 'France'),
        QT_TRANSLATE_NOOP('Country', 'Georgia'),
        QT_TRANSLATE_NOOP('Country', 'Germany'),
        QT_TRANSLATE_NOOP('Country', 'Greece'),
        QT_TRANSLATE_NOOP('Country', 'Hong Kong'),
        QT_TRANSLATE_NOOP('Country', 'Hungary'),
        QT_TRANSLATE_NOOP('Country', 'Iceland'),
        QT_TRANSLATE_NOOP('Country', 'India'),
        QT_TRANSLATE_NOOP('Country', 'Indonesia'),
        QT_TRANSLATE_NOOP('Country', 'Ireland'),
        QT_TRANSLATE_NOOP('Country', 'Israel'),
        QT_TRANSLATE_NOOP('Country', 'Italy'),
        QT_TRANSLATE_NOOP('Country', 'Japan'),
        QT_TRANSLATE_NOOP('Country', 'Latvia'),
        QT_TRANSLATE_NOOP('Country', 'Luxembourg'),
        QT_TRANSLATE_NOOP('Country', 'Malaysia'),
        QT_TRANSLATE_NOOP('Country', 'Mexico'),
        QT_TRANSLATE_NOOP('Country', 'Moldova'),
        QT_TRANSLATE_NOOP('Country', 'Netherlands'),
        QT_TRANSLATE_NOOP('Country', 'New Zealand'),
        QT_TRANSLATE_NOOP('Country', 'North Macedonia'),
        QT_TRANSLATE_NOOP('Country', 'Norway'),
        QT_TRANSLATE_NOOP('Country', 'Poland'),
        QT_TRANSLATE_NOOP('Country', 'Portugal'),
        QT_TRANSLATE_NOOP('Country', 'Romania'),
        QT_TRANSLATE_NOOP('Country', 'Serbia'),
        QT_TRANSLATE_NOOP('Country', 'Singapore'),
        QT_TRANSLATE_NOOP('Country', 'Slovakia'),
        QT_TRANSLATE_NOOP('Country', 'Slovenia'),
        QT_TRANSLATE_NOOP('Country', 'South Africa'),
        QT_TRANSLATE_NOOP('Country', 'South Korea'),
        QT_TRANSLATE_NOOP('Country', 'Spain'),
        QT_TRANSLATE_NOOP('Country', 'Sweden'),
        QT_TRANSLATE_NOOP('Country', 'Switzerland'),
        QT_TRANSLATE_NOOP('Country', 'Taiwan'),
        QT_TRANSLATE_NOOP('Country', 'Thailand'),
        QT_TRANSLATE_NOOP('Country', 'Turkey'),
        QT_TRANSLATE_NOOP('Country', 'Ukraine'),
        QT_TRANSLATE_NOOP('Country', 'United Arab Emirates'),
        QT_TRANSLATE_NOOP('Country', 'United Kingdom'),
        QT_TRANSLATE_NOOP('Country', 'United States'),
        QT_TRANSLATE_NOOP('Country', 'Vietnam'),
    ]

    // Translations of all city names used by NordVPN (as of 2020-11-08).
    // Usage: qsTranslate('City', <your country name or variable here>)
    readonly property var countries: [
        QT_TRANSLATE_NOOP('City', 'Adelaide'),
        QT_TRANSLATE_NOOP('City', 'Amsterdam'),
        QT_TRANSLATE_NOOP('City', 'Athens'),
        QT_TRANSLATE_NOOP('City', 'Atlanta'),
        QT_TRANSLATE_NOOP('City', 'Auckland'),
        QT_TRANSLATE_NOOP('City', 'Bangkok'),
        QT_TRANSLATE_NOOP('City', 'Belgrad'),
        QT_TRANSLATE_NOOP('City', 'Berlin'),
        QT_TRANSLATE_NOOP('City', 'Bratislava'),
        QT_TRANSLATE_NOOP('City', 'Brisbane'),
        QT_TRANSLATE_NOOP('City', 'Brussels'),
        QT_TRANSLATE_NOOP('City', 'Bucharest'),
        QT_TRANSLATE_NOOP('City', 'Budapest'),
        QT_TRANSLATE_NOOP('City', 'Buenos Aires'),
        QT_TRANSLATE_NOOP('City', 'Buffalo'),
        QT_TRANSLATE_NOOP('City', 'Charlotte'),
        QT_TRANSLATE_NOOP('City', 'Chennai'),
        QT_TRANSLATE_NOOP('City', 'Chicago'),
        QT_TRANSLATE_NOOP('City', 'Chisinau'),
        QT_TRANSLATE_NOOP('City', 'Copenhagen'),
        QT_TRANSLATE_NOOP('City', 'Dallas'),
        QT_TRANSLATE_NOOP('City', 'Denver'),
        QT_TRANSLATE_NOOP('City', 'Dubai'),
        QT_TRANSLATE_NOOP('City', 'Dublin'),
        QT_TRANSLATE_NOOP('City', 'Frankfurt'),
        QT_TRANSLATE_NOOP('City', 'Hanoi'),
        QT_TRANSLATE_NOOP('City', 'Helsinki'),
        QT_TRANSLATE_NOOP('City', 'Hong Kong'),
        QT_TRANSLATE_NOOP('City', 'Istanbul'),
        QT_TRANSLATE_NOOP('City', 'Jakarta'),
        QT_TRANSLATE_NOOP('City', 'Johannesburg'),
        QT_TRANSLATE_NOOP('City', 'Kiev'),
        QT_TRANSLATE_NOOP('City', 'Kuala Lumpur'),
        QT_TRANSLATE_NOOP('City', 'Lisbon'),
        QT_TRANSLATE_NOOP('City', 'Ljubljana'),
        QT_TRANSLATE_NOOP('City', 'London'),
        QT_TRANSLATE_NOOP('City', 'Los Angeles'),
        QT_TRANSLATE_NOOP('City', 'Madrid'),
        QT_TRANSLATE_NOOP('City', 'Manassas'),
        QT_TRANSLATE_NOOP('City', 'Melbourne'),
        QT_TRANSLATE_NOOP('City', 'Mexico'),
        QT_TRANSLATE_NOOP('City', 'Miami'),
        QT_TRANSLATE_NOOP('City', 'Milan'),
        QT_TRANSLATE_NOOP('City', 'Montreal'),
        QT_TRANSLATE_NOOP('City', 'Mumbai'),
        QT_TRANSLATE_NOOP('City', 'New York'),
        QT_TRANSLATE_NOOP('City', 'Nicosia'),
        QT_TRANSLATE_NOOP('City', 'Oslo'),
        QT_TRANSLATE_NOOP('City', 'Paris'),
        QT_TRANSLATE_NOOP('City', 'Perth'),
        QT_TRANSLATE_NOOP('City', 'Phoenix'),
        QT_TRANSLATE_NOOP('City', 'Prague'),
        QT_TRANSLATE_NOOP('City', 'Reykjavik'),
        QT_TRANSLATE_NOOP('City', 'Riga'),
        QT_TRANSLATE_NOOP('City', 'Saint Louis'),
        QT_TRANSLATE_NOOP('City', 'Salt Lake City'),
        QT_TRANSLATE_NOOP('City', 'San Francisco'),
        QT_TRANSLATE_NOOP('City', 'San Jose'),
        QT_TRANSLATE_NOOP('City', 'San Paulo'),
        QT_TRANSLATE_NOOP('City', 'Santiago'),
        QT_TRANSLATE_NOOP('City', 'Sarajevo'),
        QT_TRANSLATE_NOOP('City', 'Seattle'),
        QT_TRANSLATE_NOOP('City', 'Seoul'),
        QT_TRANSLATE_NOOP('City', 'Singapore'),
        QT_TRANSLATE_NOOP('City', 'Skopje'),
        QT_TRANSLATE_NOOP('City', 'Sofia'),
        QT_TRANSLATE_NOOP('City', 'Steinsel'),
        QT_TRANSLATE_NOOP('City', 'Stockholm'),
        QT_TRANSLATE_NOOP('City', 'Sydney'),
        QT_TRANSLATE_NOOP('City', 'Taipei'),
        QT_TRANSLATE_NOOP('City', 'Tallinn'),
        QT_TRANSLATE_NOOP('City', 'Tbilisi'),
        QT_TRANSLATE_NOOP('City', 'Tel Aviv'),
        QT_TRANSLATE_NOOP('City', 'Tirana'),
        QT_TRANSLATE_NOOP('City', 'Tokyo'),
        QT_TRANSLATE_NOOP('City', 'Toronto'),
        QT_TRANSLATE_NOOP('City', 'Vancouver'),
        QT_TRANSLATE_NOOP('City', 'Vienna'),
        QT_TRANSLATE_NOOP('City', 'Warsaw'),
        QT_TRANSLATE_NOOP('City', 'Zagreb'),
        QT_TRANSLATE_NOOP('City', 'Zurich'),
    ]
}
