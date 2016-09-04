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
    //QSize newSize = loadedImage.size().expandedTo(size());
    //resizeImage(&loadedImage, newSize);
    image = loadedImage;
    // and add an image to the slider
    array[array_counter]=fileName;
    array_image[array_counter]=image;
    array_counter++;
    slider->setMaximum(array_counter-1);


    modified = false;
    update();

    return true;
}

/*bool scribble::openFolderDicom(const QString &directoryName)
{
    QImage loadedImage;
    QDir myDir(directoryName);
    //myDir.setSorting(QDir::Unsorted);
    QStringList list_all_files=myDir.entryList();
    for(int i=0;i<list_all_files.length();i++)
    {
        if(isDicom(list_all_files.at(i).toStdString()))
        {
            std::cout<<"here the dicom image called: "<<list_all_files.at(i).toStdString()<<std::endl;
            std::string fileName=directoryName.toStdString()+"/"+list_all_files.at(i).toStdString();
            // transform the name of the file from .dcm to .jpg and the file from dicom to jpeg and load the new jpeg image
            std::string fileName1_str=changeDicomToJpeg(fileName);
            std::cout<<"le filename dicom est: "<<fileName<<" alors que le jpg filename est "<<fileName1_str<<std::endl;
            transformation(fileName,fileName1_str);
            QString fileName1(fileName1_str.c_str());
            if (!loadedImage.load(fileName1)){
                return false;
            }
            //resize the image to the size of the scribble
            //QSize newSize = loadedImage.size().expandedTo(size());
            //resizeImage(&loadedImage, newSize);
            image = loadedImage;

            // and an image to the slider
            array[array_counter]=list_all_files.at(i);
            std::cout<<"array_counter est de "<<array_counter<<" et ce file a pour nom: "<<array[array_counter].toStdString()<<std::endl;
            array_image[array_counter]=image;
            array_counter++;
            slider->setMaximum(array_counter-1);

            modified = false;
            update();
        }
    }
    slider->setSliderPosition(slider->maximum());
    return true;
}*/

bool scribble::openFolderDicom(const QString &directoryName)
{
    std::string command="ipython C:/Users/User/Desktop/displaydicom.py ";
    command.append("C:/Users/User/Desktop/All_liver/TCGA-LIHC/TCGA-BC-A3KG/1.3.6.1.4.1.14519.5.2.1.8421.4008.303533339368406310446855637599/1.3.6.1.4.1.14519.5.2.1.8421.4008.807354510232772395197490232364");
    system(command.c_str());
    QString directoryName1=QString::fromStdString("C:/Users/User/Desktop/DicomImages");
    QImage loadedImage;
    QDir myDir(directoryName1);
    QStringList list_all_files=myDir.entryList();
    for(int i=0;i<list_all_files.length();i++)
    {
        if(isJpg(list_all_files.at(i).toStdString()) || isPng(list_all_files.at(i).toStdString())){
            std::string fileName=directoryName1.toStdString()+"/"+list_all_files.at(i).toStdString();
            QString fileName1(fileName.c_str());
            if (!loadedImage.load(fileName1)){
                return false;
            }
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
    return true;
}

bool scribble::openFolder(const QString &directoryName)
{
    QImage loadedImage;
    QDir myDir(directoryName);
    QStringList list_all_files=myDir.entryList();
    for(int i=0;i<list_all_files.length();i++)
    {
        if(isJpg(list_all_files.at(i).toStdString()) || isPng(list_all_files.at(i).toStdString())){
            std::string fileName=directoryName.toStdString()+"/"+list_all_files.at(i).toStdString();
            QString fileName1(fileName.c_str());
            if (!loadedImage.load(fileName1)){
                return false;
            }
            //resize the image to the size of the scribble
            //QSize newSize = loadedImage.size().expandedTo(size());
            //resizeImage(&loadedImage, newSize);
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
    return true;
}

bool scribble::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = image;
    //resizeImage(&visibleImage, size());

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
    if (event->button() == Qt::LeftButton && image_loaded && myPenColor!=Qt::red) {
        lastPoint = event->pos();
        scribbling = true;
    }
    if(myPenColor==Qt::red && countPoints<2)
    {
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
        for(int i=0;i<slider->maximum()+1;i++){
            image=array_image[i];
            drawSimpleLineTo(pointABis,pointBBis);
            array_image[i]=image;
        }
    }
}

void scribble::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

/*void scribble::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}*/

void scribble::drawSimpleLineTo(const QPoint &a, const QPoint &b)
{
    QPainter painter(&image);
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

/*void scribble::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}*/

void scribble::display_image(int number_image)
{
    //QImage loadedImage;
    //loadedImage.load(array[number_image]);
    //QSize newSize = /*loadedImage*/array_image[number_image].size().expandedTo(size());
    //resizeImage(&array_image[number_image]/*&loadedImage*/, newSize);
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
double scribble::computeAlgorithm2(int margin, double percentage, int part)
{
    double volume_final=0;
    double volume_liver=0;
    double volume_tumor=0;
    for(int i=0;i<array_counter;i++)
    {
        double increase_volume=getVolumeAfterSeparation(array_image[i],part,percentage);
        if(increase_volume<0)
        {
            return -1;
        }
        volume_liver=volume_liver+increase_volume;
        volume_tumor=volume_tumor+getVolumeTumor(array_image[i],margin);
    }
    volume_final=volume_tumor/(volume_tumor+volume_liver);
    return volume_final;
}
