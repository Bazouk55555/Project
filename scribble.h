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
    QImage getImage();
    bool openFile(const QString &fileName);
    bool openFolder(const QString &directoryName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);
    void setImageLoaded(bool loaded);
    double computeAlgorithm1(int margin);
    double computeAlgorithm2(int margin, int part);

    bool isModified() const { return modified; }
    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }

public slots:
    void clearImage();
    void display_image(int);

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);

    bool image_loaded;
    bool modified;
    bool scribbling;
    int myPenWidth;
    QColor myPenColor;
    QImage image;
    QImage array_image[200];
    QString array[200];
    int array_counter;
    QPoint lastPoint;
    QSlider *slider;

};

#endif
