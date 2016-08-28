#include <QtWidgets>
#include <QPrinter>
#include <QPrintDialog>

#include "compute_volume.h"
#include "scribble.h"
#include "transform_2_jpeg.h"

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
    countPoints=0;
    QObject::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(display_image(int))) ;
    //std::cout<<"mais la au const size vaut ("<<size().width()<<","<<size().height()<<")"<<std::endl;

}

QImage scribble::getImage()
{
    return image;
}

//adding a open image and change the current openimage with openfile
bool scribble::openFile(const QString &fileName)
{
    QImage loadedImage;
    if (!loadedImage.load(fileName)){
        return false;
    }
        //resize the image to the size of the scribble
    //std::cout<<"1: la width: "<<loadedImage.width()<<" et la height "<<loadedImage.height()<<std::endl;
    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    //std::cout<<"2:la widht: "<<loadedImage.width()<<" et la height "<<loadedImage.height()<<std::endl;
    //std::cout<<"et size vaut ("<<size().width()<<","<<size().height()<<")"<<std::endl;
    //std::cout<<"et scribble vaut ("<<this->size().width()<<","<<this->size().height()<<")"<<std::endl;
    image = loadedImage;
    //std::cout<<"2:la image widht: "<<image.width()<<" et la height "<<image.height()<<std::endl;
    // and add an image to the slider
    array[array_counter]=fileName;
    array_image[array_counter]=image;
    array_counter++;
    slider->setMaximum(array_counter-1);


    modified = false;
    update();

    return true;
}

bool scribble::openFolderDicom(const QString &directoryName)
{
    QImage loadedImage;
    QDir myDir(directoryName);
    //myDir.setSorting(QDir::Unsorted);
    QStringList list_all_files=myDir.entryList();
    for(int i=0;i<list_all_files.length();i++)
    {
        if(isDicom(list_all_files.at(i).toStdString()))
        {
            std::cout<<"here the dicon image called: "<<list_all_files.at(i).toStdString()<<std::endl;
            std::string fileName=directoryName.toStdString()+"/"+list_all_files.at(i).toStdString();
            // transform the name of the file from .dcm to .jpg and the file from dicom to jpeg and load the new jpeg image
            std::string fileName1_str=changeDicomToJpeg(fileName);
            transformation(fileName,fileName1_str);
            QString fileName1(fileName1_str.c_str());
            if (!loadedImage.load(fileName1)){
                return false;
            }
            //resize the image to the size of the scribble
            QSize newSize = loadedImage.size().expandedTo(size());
            resizeImage(&loadedImage, newSize);
            image = loadedImage;

            // and an image to the slider
            array[array_counter]=list_all_files.at(i);
            array_image[array_counter]=image;
            array_counter++;
            slider->setMaximum(array_counter-1);

            modified = false;
            update();
        }
    }
    slider->setSliderPosition(slider->maximum());
    //std::cout<<"the width of the image is: "<<image.width()<<" and the height is: "<<image.height()<<std::endl;
    return true;
}

bool scribble::openFolder(const QString &directoryName)
{
    QImage loadedImage;
    QDir myDir(directoryName);
    QStringList list_all_files=myDir.entryList();
    for(int i=0;i<list_all_files.length();i++)
    {
        std::cout<<list_all_files.at(i).toStdString()<<std::endl;
        if(isJpg(list_all_files.at(i).toStdString()) || isPng(list_all_files.at(i).toStdString())){
            std::string fileName=directoryName.toStdString()+"/"+list_all_files.at(i).toStdString();
            QString fileName1(fileName.c_str());
            if (!loadedImage.load(fileName1)){
                return false;
            }
            //resize the image to the size of the scribble
            QSize newSize = loadedImage.size().expandedTo(size());
            resizeImage(&loadedImage, newSize);
            image = loadedImage;

            // and an image to the slider
            array[array_counter]=list_all_files.at(i);
            array_image[array_counter]=image;
            array_counter++;
            slider->setMaximum(array_counter-1);

        modified = false;
        update();
        }
    }
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
    QImage *imagebis=new QImage();
    image=*imagebis;
    for(int i=0;i<array_counter;i++)
    {
        array_image[i]=*imagebis;
        array[i]="";
    }
    array_counter=0;
    slider->setMaximum(array_counter);
    delete imagebis;
}

int scribble::getCountPoints(){
    return countPoints;
}

void scribble::mousePressEvent(QMouseEvent *event)
{
    //std::cout<<"je suis a 3"<<std::endl;
    if (event->button() == Qt::LeftButton && image_loaded && myPenColor!=Qt::red) {
        lastPoint = event->pos();
        scribbling = true;
    }
    //std::cout<<"je suis a 4"<<std::endl;
    if(myPenColor==Qt::red && countPoints<2)
    {
        //std::cout<<"je suis a 5"<<std::endl;
        drawPointTo(event->pos());
        if(countPoints==0)
        {
            pointA.setX(event->pos().x());
            pointA.setY(event->pos().y());
        }
        if(countPoints==1)
        {
            pointB.setX(event->pos().x());
            pointB.setY(event->pos().y());
        }
        //std::cout<<"je suis a 6"<<std::endl;
    }
}

void scribble::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling && image_loaded && event->pos().x()<image.size().height()&&event->pos().y()<image.size().width() && myPenColor!=Qt::red)
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
    if (myPenColor==Qt::red && countPoints<2)
    {
        countPoints++;
    }
    if (myPenColor==Qt::red && countPoints==2)
    {
        QPoint pointABis;
        QPoint pointBBis;
        pointABis.setX(0);
        pointABis.setY(pointA.y()-(pointB.y()-pointA.y())/(pointB.x()-pointA.x())*pointA.x());
        pointBBis.setX(image.width());
        pointBBis.setY(pointA.y()+(image.width()-pointA.x())*(pointB.y()-pointA.y())/(pointB.x()-pointA.x()));
        std::cout<<"le point A est:("<<pointABis.x()<<","<<pointABis.y()<<") "<<"le point B est:("<<pointBBis.x()<<","<<pointBBis.y()<<") "<<std::endl;
        drawSimpleLineTo(pointABis,pointBBis);
        array_image[slider->value()]=image;
    }
}

void scribble::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    //std::cout<<"where"<<dirtyRect.size().width()<<","<<dirtyRect.height()<<std::endl;
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

void scribble::drawSimpleLineTo(const QPoint &a, const QPoint &b)
{
    QPainter painter(&image/*&array_image[array_counter]*/);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(a, b);
    modified = true;
    update();
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

void scribble::drawPointTo(const QPoint &Point)
{
    QPainter painter(&image/*&array_image[array_counter]*/);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawPoint(Point);
    modified = true;
    update();
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

double scribble::computeAlgorithm1(int margin)
{
    double volume_final=0;
    double volume_liver=0;
    double volume_tumor=0;
    for(int i=0;i<array_counter;i++)
    {
        volume_liver=volume_liver+getVolumeLiver(array_image[i]);
        volume_tumor=volume_tumor+getVolumeTumor(array_image[i],margin);
    }
    volume_final=volume_tumor/(volume_tumor+volume_liver);
    return volume_final;
}
double scribble::computeAlgorithm2(int margin, int part)
{
    double volume_final=0;
    double volume_liver=0;
    double volume_tumor=0;
    for(int i=0;i<array_counter;i++)
    {
        volume_liver=volume_liver+getVolumeAfterSeparation(array_image[i],part);
        volume_tumor=volume_tumor+getVolumeTumor(array_image[i],margin);
    }
    volume_final=volume_tumor/(volume_tumor+volume_liver);
    return volume_final;
}
