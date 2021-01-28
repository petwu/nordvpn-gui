#include "mapimageprovider.h"

#include <QFile>
#include <QIODevice>
#include <QImage>
#include <QPainter>
#include <QSvgRenderer>
#include <QTextStream>
#include <utility>

auto MapImageProvider::requestPixmap(const QString &color, QSize *size,
                                     const QSize &requestedSize) -> QPixmap {
    QPixmap pixmap;
    // map.svg is embedded as QRC resource
    QFile file(":/img/map.svg");
    if (file.open(QIODevice::ReadOnly)) {
        // read map.svg as string
        QTextStream ts(file.readAll());
        QString svg = ts.readAll();
        // set the fill color of land areas
        svg.replace("{{ fillColor }}", color);

        // prepare svg renderer to draw the image
        QSvgRenderer svgRenderer(svg.toUtf8());
        // probably only one dimension, width or height, is set, so we defer the
        // other one based on the SVGs aspect ratio
        QSize defaultSize = svgRenderer.defaultSize();
        double ratio = (double)defaultSize.width() / defaultSize.height();
        int width = requestedSize.width();
        int height = requestedSize.height();
        if (width <= 0) {
            width = height * ratio;
        } else if (height <= 0) {
            height = width / ratio;
        }

        // render svg as pixmap
        QImage image(width, height, QImage::Format_ARGB32);
        image.fill(0x00000000);
        QPainter painter(&image);
        svgRenderer.render(&painter, image.rect());
        pixmap = QPixmap::fromImage(image);
    }
    file.close();
    return std::move(pixmap);
}
