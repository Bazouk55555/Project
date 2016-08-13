#include "compute_volume.h"
#include <iostream>

int getVolumeLiver(QImage image_computed){

    QColor **color=new QColor*[image_computed.size().width()];
    QRgb **s=new QRgb*[image_computed.size().width()];
    int volume=0;

    // Go through the width of the image
    for(int i=0;i<image_computed.size().width();i++)
    {
        s[i]=new QRgb[image_computed.size().height()];
        color[i]=new QColor[image_computed.size().height()];

        // Go through the height of the image
        for(int j=0;j<image_computed.size().height();j++)
        {
            s[i][j]=image_computed.pixel(i,j);
            color[i][j].setRgb((s[i][j]));
            QColor colorPixel=color[i][j];

            //If the color is green, add one pixel to the volume computed
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

    // Go through the width of the image
    for(int i=0;i<image_computed.size().width();i++)
    {
        s[i]=new QRgb[image_computed.size().height()];
        color[i]=new QColor[image_computed.size().height()];

        // Go through the height of the image
        for(int j=0;j<image_computed.size().height();j++)
        {
            s[i][j]=image_computed.pixel(i,j);
            color[i][j].setRgb((s[i][j]));
            QColor colorPixel=color[i][j];

            //If the color is blue, add one pixel to the volume computed
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
    bool a=false;

    // Go through the width of the image
    for(int i=0;i<image_computed.width();i++)
    {
        // Go through the height of the image
        for(int j=0;j<image_computed.height();j++)
        {
            // Check if there is a red pixel in the image to know if the separation has been maid
            if(image_computed.pixelColor(i,j).blue()<80&&image_computed.pixelColor(i,j).green()<80&&image_computed.pixelColor(i,j).red()>110)
            {
                a=true;
                break;
            }
        }
        // If the separation has been maid, a=true so we can leave the loop
        if(a==true)
        {
            break;
        }
    }
    int volume=0;

    // If the separation has been maid
    if(a==true)
    {
        QColor **color=new QColor*[image_computed.size().width()];
        QRgb **s=new QRgb*[image_computed.size().width()];

        // Go through the width of the image
        for(int i=0;i<image_computed.size().width();i++)
        {
            s[i]=new QRgb[image_computed.size().height()];
            color[i]=new QColor[image_computed.size().height()];

            // If the user has chosen the low part of the separation
            if(part==0){
                // Go through each column of the image until it reached the red separation
                for(int j=0;j<image_computed.size().height();j++){
                    s[i][j]=image_computed.pixel(i,j);
                    color[i][j].setRgb((s[i][j]));
                    QColor colorPixel=color[i][j];

                    // If there is a red pixel the red line of separation has been reached, we can go to the next column
                    if(colorPixel.red()>110&&colorPixel.blue()<80&&colorPixel.green()<80)
                    {
                        break;
                    }
                    // If the separation is still not reached, one more pixel is added to the volume
                    if(colorPixel.green()>110&&colorPixel.red()<80&&colorPixel.blue()<80)
                    {
                        volume++;
                    }
                    // If a blue pixel is reached, the wrong part of the liver has been chosen
                    if(colorPixel.green()<80&&colorPixel.red()<80&&colorPixel.blue()>110)
                    {
                        QMessageBox::information(0, "Result", "You have probably chosen the wrong part");
                        return 0;
                    }
                }
            }

            // If the user has chosen the high part of the separation(the explanation is the same for the high part)
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
    volume=getVolumeLiver(image_computed)+volume;
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
