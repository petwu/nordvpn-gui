#ifndef MAPIMAGEPROVIDER_HPP
#define MAPIMAGEPROVIDER_HPP

#include <QPixmap>
#include <QQmlEngine>
#include <QQuickImageProvider>
#include <QSize>
#include <QString>

/**
 * @brief The MapImageProvider class is responsible for providing a rendered
 * version of the SVG map.
 *
 * @details SVGs can be rendered by the QML Image control as well, but that was
 * not suitable for this use case: The color of the image (land, not water)
 * needs to be changed to fit the current (system) color theme. The colorization
 * could be done in QML via a ColorOverlay as well, but this has several
 * disadvantages:
 *
 * - The image does not look good. Especially around the borders, where a
 * ColorOverlay causes blurred/pixelated edged if color difference between
 * land and water is low.
 * - The Color overlay is part of the QtGraphicalEffects module which won't
 * be part of Qt6 (https://wiki.qt.io/New_Features_in_Qt_6.0).
 *
 * Hence we use this image provider that reads in the SVG and replaces all
 * occurences of "{{ fillColor }}" to change the color before rendering the
 * image.
 */
class MapImageProvider : public QQuickImageProvider {
  public:
    MapImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap) {}

    /**
     * @brief Render the map SVG as pixmap image.
     * @param color The color of the land masses. Needs to be a valid SVG/CSS
     * color format, preverably a hex value like #856a45.
     * @param size Not used in implementation. Can be QSize(-1, -1).
     * @param requestedSize A QSize where at least width or height are >0. The
     * other value is defered from the SVGs apsect ratio.
     * @return The rendered SVG map image with colorized land areas.
     */
    auto requestPixmap(const QString &color, QSize *size,
                       const QSize &requestedSize) -> QPixmap override;
};

#endif // MAPIMAGEPROVIDER_HPP
