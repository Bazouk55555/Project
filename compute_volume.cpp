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
            if(colorPixel.green()>110&&colorPixel.red()<80&&colorPixel.blue()<80)
            {
                volume++;
            }
        }
        delete s[i];
        delete color[i];
    }
    delete s;
    delete color;
    std::cout<<"the volume of the liver is : "<<volume<<std::endl;
    return volume;
}

int getVolumeTumor(QImage image_computed, int margin)
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
            if(colorPixel.red()<80&&colorPixel.green()<80&&colorPixel.blue()>110)
            {
                volume++;
            }
        }
        delete s[i];
        delete color[i];
    }
    delete s;
    delete color;
    volume=volume+margin;
    std::cout<<"the volume of the tumor is : "<<volume<<std::endl;
    return volume;
}

int getVolumeAfterSeparation(QImage image_computed, int part)
{
    std::cout<<"j entre dans la methode"<<std::endl;
    bool a=false;
    for(int i=0;i<image_computed.width();i++)
    {
        for(int j=0;j<image_computed.height();j++)
        {
            if(image_computed.pixelColor(i,j).blue()<80&&image_computed.pixelColor(i,j).green()<80&&image_computed.pixelColor(i,j).red()>110)
            {
                std::cout<<"a l interoeur du if pour a ==true"<<std::endl;
                a=true;
                break;
            }
        }
        if(a==true)
        {
            std::cout<<"a l interoeur du if pour a ==true bis"<<std::endl;
            break;
        }
    }
    int volume=0;
    if(a==true)
    {
        std::cout<<"il est a l interieur de la principal loop"<<std::endl;
        QColor **color=new QColor*[image_computed.size().width()];
        QRgb **s=new QRgb*[image_computed.size().width()];

        for(int i=0;i<image_computed.size().width();i++)
        {
            s[i]=new QRgb[image_computed.size().height()];
            color[i]=new QColor[image_computed.size().height()];

            // Go through the width of the image
            if(part==0){
                for(int j=0;j<image_computed.size().height();j++){
                    s[i][j]=image_computed.pixel(i,j);
                    color[i][j].setRgb((s[i][j]));
                    QColor colorPixel=color[i][j];
                    //std::cout<<"i : "<<i<<" j : "<<j<<std::endl;
                    if(colorPixel.red()>110&&colorPixel.blue()<80&&colorPixel.green()<80)
                    {
                        std::cout<<"atteint le rpouge"<<endl;
                        break;
                    }
                    if(colorPixel.green()>110&&colorPixel.red()<80&&colorPixel.blue()<80)
                    {
                        std::cout<<"augmente le volume"<<endl;
                        volume++;
                    }

                    if(colorPixel.green()<80&&colorPixel.red()<80&&colorPixel.blue()>110)
                    {
                        std::cout<<"hahahahahahahaha"<<std::endl;
                        QMessageBox::information(0, "Result", "You have probably chosen the wrong part");
                        return 0;
                    }
                }
            }
            if(part==1){
                for(int j=image_computed.size().height();j>0;j--){
                    s[i][image_computed.size().height()-j]=image_computed.pixel(i,j);
                    color[i][image_computed.size().height()-j].setRgb((s[i][image_computed.size().height()-j]));
                    QColor colorPixel=color[i][image_computed.size().height()-j];
                    //std::cout<<"i : "<<i<<" j : "<<j<<std::endl;
                    if(colorPixel.red()>110&&colorPixel.blue()<80&&colorPixel.green()<80)
                    {
                        std::cout<<"atteint le rpouge"<<endl;
                        break;
                    }
                    if(colorPixel.green()>110&&colorPixel.red()<80&&colorPixel.blue()<80)
                    {
                        std::cout<<"augmente le volume"<<endl;
                        volume++;
                    }

                    if(colorPixel.green()<80&&colorPixel.red()<80&&colorPixel.blue()>110)
                    {
                        std::cout<<"hahahahahahahaha"<<std::endl;
                        QMessageBox::information(0, "Result", "You have probably chosen the wrong part");
                        return 0;
                    }
                }
                //std::cout<<"pipi"<<std::endl;
            }
            delete s[i];
            delete color[i];
        }
        delete s;
        delete color;
    }
    std::cout<<"le volume ajoute est de : "<<volume<<std::endl;
    volume=getVolumeLiver(image_computed)+volume;
    std::cout<<"volume du liver apres increasing : "<<volume<<std::endl;
    return volume;
}

int margiumLiver(QImage image, int margium){
    int volume=0;
    int **tab=new int *[image.size().width()];
    // Put 0 in all the array;
    for(int i=0;i<image.size().width();i++)
    {
        tab[i]=new int[image.size().height()];
        for(int j=0;j<image.size().height();j++)
        {
            tab[i][j]=0;
        }
    }
    //Go through all the image
    for(int i=1;i<image.size().width();i++)
    {
        for(int j=1;j<image.size().height();j++)
        {
            //if it is a border between tumor and liver->the pixel i is blue and i-1 is green
            if(image.pixelColor(i,j).blue()>110&&image.pixelColor(i,j).red()<80&&image.pixelColor(i,j).green()<80&&image.pixelColor(i-1,j).green()>110&&image.pixelColor(i-1,j).blue()<80&&image.pixelColor(i-1,j).red()<80)
            {
                for(int k=i-1;k>i-1-margium;k--)
                {
                    //check the number of pixel from the border until the margium only if it is still the liver and add 1 to the array if it is the case
                    if(image.pixelColor(k,j).green()>110&&image.pixelColor(k,j).red()<80&&image.pixelColor(k,j).blue()<80)
                    {
                        tab[k][j]=1;
                    }
                }
            }
            //if it is a border between tumor and liver->the pixel j is blue and j-1 is green
            if(image.pixelColor(i,j).blue()>110&&image.pixelColor(i,j).green()<80&&image.pixelColor(i,j).red()<80&&image.pixelColor(i,j+1).green()>110&&image.pixelColor(i,j+1).blue()<80&&image.pixelColor(i,j+1).red()<80)
            {
                for(int k=j+1;k<j+1+margium;k++)
                {
                    //check the number of pixel from the border until the margium only if it is still the liver and add 1 to the array if it is the case
                    if(image.pixelColor(i,k).green()>110&&image.pixelColor(i,k).red()<80&&image.pixelColor(i,k).blue()<80)
                    {
                        tab[i][k]=1;
                    }
                }
            }
            if(image.pixelColor(i,j).blue()>110&&image.pixelColor(i,j).green()<80&&image.pixelColor(i,j).red()<80&&image.pixelColor(i+1,j).green()>110&&image.pixelColor(i+1,j).blue()<80&&image.pixelColor(i+1,j).red()<80)
            {
                for(int k=i+1;k<i+1+margium;k++)
                {
                    //check the number of pixel from the border until the margium only if it is still the liver and add 1 to the array if it is the case
                    if(image.pixelColor(k,j).green()>100&&image.pixelColor(k,j).red()<80&&image.pixelColor(k,j).blue()<80)
                    {
                        tab[k][j]=1;
                    }
                }
            }
            if(image.pixelColor(i,j).blue()>110&&image.pixelColor(i,j).green()<80&&image.pixelColor(i,j).red()<80&&image.pixelColor(i,j-1).green()>110&&image.pixelColor(i,j-1).blue()<80&&image.pixelColor(i,j-1).red()<80)
            {
                for(int k=j-1;k>j-1-margium;k--)
                {
                    //check the number of pixel from the border until the margium only if it is still the liver and add 1 to the array if it is the case
                    if(image.pixelColor(i,k).green()>110&&image.pixelColor(i,k).red()<80&&image.pixelColor(i,k).blue()<80)
                    {
                        tab[i][k]=1;
                    }
                }
            }
        }
    }
    //Compute the number of 1 in the array
    for(int i=0;i<image.size().width();i++)
    {
        for(int j=0;j<image.size().height();j++)
        {
            if(tab[i][j]==1)
            {
                volume++;
            }
        }
        delete tab[i];
    }
    delete tab;
    return volume;
}
