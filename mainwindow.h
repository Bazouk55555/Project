#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>

class scribble;

class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow();
    ~mainwindow();

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void openFolderDicom();
    void openFolder();
    void openFile();
    void save();
    void displayIn3D();
    void displayJPGIn3D();
    void segment();
    void enterDatas();
    void computeAlgorithm1();
    void computeAlgorithm2();
    void selectTumor();
    void selectLiver();
    void separationLiver();
    void separationLiverBis();
    void penWidth();

private:
    void createActions();
    void createMenus();
    bool maybeSave();
    bool saveFile(const QByteArray &fileFormat);
    std::string directory_dicom;
    QString directory_segmentation;

    scribble *scribbleArea;

    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QAction *openDicom;
    QAction *openAct;
    QAction *openAct2;
    QList<QAction *> saveAsActs;
    QAction *exitAct;
    QAction *display_dicom_in_3d;
    QAction *display_segmentation_in_3d;
    QAction *segmentation;
    QAction *input_datas;
    QAction *compute_algorithm1;
    QAction *compute_algorithm2;
    QAction *technique2;
    QAction *select_tumor;
    QAction *select_liver;
    QAction *penWidthAct;
    QAction *clearScreenAct;

    double volume;
    double margin;
};

#endif
