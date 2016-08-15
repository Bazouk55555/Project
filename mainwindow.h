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

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void openFolder();
    void openFile();
    void save();
    void segment();
    void enterDatas();
    void computeAlgorithm1();
    void computeAlgorithm2();
    void selectTumor();
    void selectLiver();
    void separationLiver();
    void penWidth();

private:
    void createActions();
    void createMenus();
    bool maybeSave();
    bool saveFile(const QByteArray &fileFormat);

    scribble *scribbleArea;

    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;

    QAction *openAct;
    QAction *openAct2;
    QList<QAction *> saveAsActs;
    QAction *exitAct;
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
