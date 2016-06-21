#include <QtWidgets>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#include <QPrintDialog>
#endif

#include "scribble.h"

scribble::scribble(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    modified = false;
    scribbling = false;
    image_loaded=false;
    myPenWidth = 1;
    myPenColor = Qt::blue;
}

bool scribble::openImage(const QString &fileName)
{
    QImage loadedImage;
    if (!loadedImage.load(fileName)){
        return false;
    }
    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}

bool scribble::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    } else {
        return false;
    }
}

void scribble::setImageLoaded(bool loaded)
{
    image_loaded=loaded;
}

void scribble::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void scribble::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void scribble::clearImage()
{
    image.fill(qRgb(255, 255, 255));
    modified = true;
    update();
}

void scribble::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && image_loaded) {
        lastPoint = event->pos();
        scribbling = true;
    }
}

void scribble::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling && image_loaded)
        drawLineTo(event->pos());
}

void scribble::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling && image_loaded) {
        drawLineTo(event->pos());
        scribbling = false;
    }
}

void scribble::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

void scribble::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void scribble::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;
    update();
    lastPoint = endPoint;
}

void scribble::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}


