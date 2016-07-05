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
    void open();
    void save();
    void enterDatas();
    void computeAlgorithm();
    void selectTumor();
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
    QList<QAction *> saveAsActs;
    QAction *exitAct;
    QAction *input_datas;
    QAction *compute_algorithm;
    QAction *select_tumor;
    QAction *penWidthAct;
    QAction *clearScreenAct;

    QString volume;
    QString margin;
};

#endif
