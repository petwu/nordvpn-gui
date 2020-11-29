/**
 * @page resources Resources
 *
 * @brief This page describes the different types of resources, both internal
 * and external.
 *
 * @section resources_internal Internal Resources
 *
 * Resources that are part of the repository are called internal resources. They
 * can be found in the `res/` directory.
 *
 * The internal resources are further devided into subdirectories depending on
 * the type of resource. Those include:
 * - `creadits`: JavaScript modules containing attributions to Open-Source
 * packages used by this application. They are displayed in the credits window
 * of the application.
 * - `data`: Hard-coded application data.
 * - `fonts`, `icons`, `img`: self-explanatory
 * - `translations`: Qt translation files.
 *
 * Some of those resources like images or fonts are compiled into the
 * application using the Qt Resource System
 * (https://doc.qt.io/qt-5/resources.html). If so, they are listed in a `.qrc`
 * file (Qt resource compiler).
 *
 * @subsection resources_style Style module
 *
 * The `res/Style/Style.qml` and `res/Style/qmldir` files create a QML module
 * that can be imported in a `.qml` file with
 * @code
 * import Style 1.0
 * @endcode
 * Therefor the files must be added to a `.qrc` resource file
 * @code
 * <qresource prefix="/Style">
 *     <file alias="Style/Style.qml">Style/Style.qml</file>
 *     <file alias="Style/qmldir">Style/qmldir</file>
 * </qresource>
 * @endcode
 * and can then included the C++ source code using
 * @code
 * QQmlApplicationEngine engine;
 * engine.addImportPath(":/Style");
 * @endcode
 * The Style module is a singleton providing properties that should/can be used
 * globally. Those include e.g. most colors used in the applicaion. They are
 * defered from system colors, which is why the application will adapt it's look
 * to the current system (Windows, macOS, KDE, Gnome, ...) as well as to the
 * current color scheme (dark, light, solarized, ...).
 *
 * @subsection resources_translations Translations
 *
 * The application supports being translated into multiple languages. The
 * translation mechanism is provided by the Qt Linguist toolset. See:
 * - https://doc.qt.io/qt-5/qtlinguist-index.html
 * - https://doc.qt.io/qt-5/qtquick-internationalization.html
 * - https://doc.qt.io/qt-5/i18n-source-translation.html
 *
 * To be able to translate text properly it is important to use functions
 * provided by Qt like `tsTr()` where ever a text should be translated. Strings
 * that are not hard-coded but provided by some mediator (see @ref
 * architecture_layers_logic) at runtime, e.g. a country name, those strings can
 * be listed in `src/ui/Translations.qml` using the `QT_TRANSLATE_NOOP()` macro
 * so that the can be found and translated by the Qt Linguist toolchain.
 *
 * @note For Developers: There is a README in `res/translations` which explains
 * the process of adding or updating translations in further details.
 *
 * @section resources_external External Resources
 *
 * Resources that are not part of the repository but fetched from external
 * sources at runtime are called external resources.
 *
 * @subsection resources_nordvpn_api NordVPN API
 *
 * The primary external resource is NordVPNs API.
 *
 */
