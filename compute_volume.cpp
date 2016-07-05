#include "compute_volume.h"
#include <iostream>

int getVolumeLiver(QImage image_computed){

    // Go through the height of the image
    QColor **color=new QColor*[image_computed.size().width()];
    QRgb **s=new QRgb*[image_computed.size().width()];
    int volume=0;
    for(int i=0;i<image_computed.size().width();i++)
    {
        s[i]=new QRgb[image_computed.size().height()];
        color[i]=new QColor[image_computed.size().height()];

        // Go through the width of the image
        for(int j=0;j<image_computed.size().height();j++)
        {
            s[i][j]=image_computed.pixel(i,j);
            color[i][j].setRgb((s[i][j]));
            QColor colorPixel=color[i][j];
            if(colorPixel.green()>100&&colorPixel.red()<50&&colorPixel.blue()<50)
            {
                volume++;
            }
        }
    }
    return volume;
}

int getVolumeTumor(QImage image_computed)
{
    QColor **color=new QColor*[image_computed.size().width()];
    QRgb **s=new QRgb*[image_computed.size().width()];
    int volume=0;

    for(int i=0;i<image_computed.size().width();i++)
    {
        s[i]=new QRgb[image_computed.size().height()];
        color[i]=new QColor[image_computed.size().height()];

        // Go through the width of the image
        for(int j=0;j<image_computed.size().height();j++)
        {
            s[i][j]=image_computed.pixel(i,j);
            color[i][j].setRgb((s[i][j]));
            QColor colorPixel=color[i][j];
            if(colorPixel.red()>100&&colorPixel.green()<50&&colorPixel.blue()<50)
            {
                volume++;
            }
        }
    }
    return volume;
}
