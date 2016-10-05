#ifndef SCRIBBLE_H
#define SCRIBBLE_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QSlider>
#include <iostream>

class scribble : public QWidget
{
    Q_OBJECT

public:
    scribble(QWidget *parent = 0);
    ~scribble();

    //Return the image
    QImage getImage();

    bool openFolderDicom(const QString &directoryName);

    // Open a jpg or png file
    bool openFile(const QString &fileName);

    // Open a folder with jpg or png images
    bool openFolder(const QString &directoryName);

    // Save the result
    bool saveImage(const QString &fileName, const char *fileFormat);

    // Set the color of an eventually pen
    void setPenColor(const QColor &newColor);

    // Set the width of an eventually pen
    void setPenWidth(int newWidth);

    // Indicate if an image is loaded
    void setImageLoaded(bool loaded);

    // Compute the algorithm for the ablation
    double computeAlgorithm1(int margin);

    // Compute the algorithm for the PVE
    double computeAlgorithm2(int margin, double percentage, int part);

    // Indicate if modifications have been made
    bool isModified() const { return modified; }

    // Return the color of the pen
    QColor penColor() const { return myPenColor; }

    // Return the width of the pen
    int penWidth() const { return myPenWidth; }

    int getCountPoints();
    void drawSimpleLineTo(const QPoint &a, const QPoint &b);

public slots:

    // Clear the image
    void clearImage();

    // Clear all the images
    //void clearEverything();

    // Display the image
    void display_image(int);

protected:

    // Start painting when the mouse is pressed
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    // Paint at the position of the mouse
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    // Stop painting when the mouse is released
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    // Overwrite the method paintEven
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    // Method to resize
    //void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:

    // Draw at the point indicated
    void drawLineTo(const QPoint &endPoint);

    void drawPointTo(const QPoint &Point);

    // Resize the image
    void resizeImage(QImage *image, const QSize &newSize);

    // Boolean that indicates if an image has been loaded
    bool image_loaded;

    // Boolean that indicate if the scribble area has been modified
    bool modified;

    // Boolean that indicates if it is possible to paint
    bool scribbling;

    int myPenWidth;
    QColor myPenColor;
    QImage image;
    QImage array_image[200];
    QString array[200];
    int array_counter;
    QPoint lastPoint;
    QSlider *slider;
    int countPoints;
    QPoint pointA;
    QPoint pointB;

};

#endif
