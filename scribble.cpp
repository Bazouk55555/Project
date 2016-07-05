#include <QtWidgets>
#include <QPrinter>
#include <QPrintDialog>

#include "compute_volume.h"
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
    slider=new QSlider(Qt::Horizontal,this);
    array_counter=0;
    slider->setMaximum(array_counter);
    QObject::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(display_image(int))) ;

}

QImage scribble::getImage()
{
    return image;
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
    array[array_counter]=fileName;
    array_image[array_counter]=image;
    array_counter++;
    slider->setMaximum(array_counter-1);
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
    if ((event->buttons() & Qt::LeftButton) && scribbling && image_loaded && event->pos().x()<image.size().height()&&event->pos().y()<image.size().width())
    {
        drawLineTo(event->pos());
    }
}

void scribble::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling && image_loaded) {
        drawLineTo(event->pos());
        scribbling = false;
        array_image[slider->value()]=image;
    }
}

void scribble::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image/*array_image[array_counter]*/, dirtyRect);
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
    QPainter painter(&image/*&array_image[array_counter]*/);
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

void scribble::display_image(int number_image)
{
    //QImage loadedImage;
    //loadedImage.load(array[number_image]);
    QSize newSize = /*loadedImage*/array_image[number_image].size().expandedTo(size());
    resizeImage(&array_image[number_image]/*&loadedImage*/, newSize);
    image = /*loadedImage*/array_image[number_image];
    update();
}

double scribble::computeAlgorithm()
{
    double volume_final=0;
    for(int i=0;i<array_counter;i++)
    {
        volume_final= volume_final+getVolumeTumor(array_image[i])/(double)(getVolumeLiver(array_image[i])+getVolumeTumor(array_image[i]));
    }
    volume_final=volume_final/array_counter;
    std::cout<<"Surface of the liver: "<<volume_final<<" m3"<<endl;
    return volume_final;
}
