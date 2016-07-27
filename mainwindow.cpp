#include <QtWidgets>
#include <iostream>
#include "mainwindow.h"
#include "scribble.h"

mainwindow::mainwindow()
{
    scribbleArea = new scribble;
    setCentralWidget(scribbleArea);

    createActions();
    createMenus();

    setWindowTitle(tr("Liver Surgery Helping"));
    resize(500, 500);
}

void mainwindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

//To open a folder with Dicom images
void mainwindow::openFolder()
{
    if (maybeSave()) {
        QString directory = QFileDialog::getExistingDirectory(this);
        if (!directory.isEmpty())
        {
            scribbleArea->openFolder(directory);
            input_datas->setEnabled(true);
        }
    }
}

//To open a Dicom image file
void mainwindow::openFile()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
        {
            scribbleArea->openFile(fileName);
            input_datas->setEnabled(true);
        }
    }
}

void mainwindow::save()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

//To enter the datas margin and volume expected
void mainwindow::enterDatas()
{
    bool ok=false;
    double volume_entered= QInputDialog::getDouble(this,"volume of the liver", "Enter the volume of the liver remaining at the end", 0,0,100,2, &ok);
    if (ok)
    {
        ok=false;
        double margin_entered= QInputDialog::getDouble(this,"Margin", "Enter the margin", 0,0,2,2, &ok);
        if(ok)
        {
            volume=volume_entered;
            margin=margin_entered;
            select_tumor->setEnabled(true);
            penWidthAct->setEnabled(true);
            return;
        }
        else
        {
            return;
        }
    }
    else
    {
        return;
    }
}

void mainwindow::computeAlgorithm()
{
    std::cout<<"Le volume tech1 est de "<<scribbleArea->computeAlgorithm1()<<std::endl;
    if(scribbleArea->computeAlgorithm1()>volume)
    {
        QMessageBox::information(this, "Result", "Tee volume remaining is "/*+scribbleArea->computeAlgorithm1()+". You can simply do the technique number 1"*/);
        return;
    }
    if(scribbleArea->computeAlgorithm2()>volume)
    {
        QMessageBox::information(this, "Result", "Tee volume remaining is "/*+scribbleArea->computeAlgorithm2()+". You can simply do the technique number 2"*/);
        return;
    }
}

void mainwindow::selectTumor()
{
    scribbleArea->setImageLoaded(true);
    scribbleArea->setPenColor(Qt::red);
    compute_algorithm->setEnabled(true);
}

void mainwindow::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Scribble"),
                                        tr("Select pen width:"),
                                        scribbleArea->penWidth(),
                                        1, 50, 1, &ok);
    if (ok)
        scribbleArea->setPenWidth(newWidth);
}

void mainwindow::createActions()
{
    openAct = new QAction(tr("&Open Folder"), this);
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFolder()));

    openAct2 = new QAction(tr("&Open File"), this);
    connect(openAct2, SIGNAL(triggered()), this, SLOT(openFile()));

    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());

        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(save()));
        saveAsActs.append(action);
    }

    exitAct = new QAction(tr("&Exit"), this);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    input_datas= new QAction(tr("&Input Data"), this);
    input_datas->setEnabled(false);
    connect(input_datas, SIGNAL(triggered()), this, SLOT(enterDatas()));

    select_tumor=new QAction(tr("&Select Tumor"), this);
    select_tumor->setEnabled(false);
    connect(select_tumor, SIGNAL(triggered()), this, SLOT(selectTumor()));

    compute_algorithm=new QAction(tr("&Compute algorithm"), this);
    compute_algorithm->setEnabled(false);
    connect(compute_algorithm, SIGNAL(triggered()), this, SLOT(computeAlgorithm()));

    penWidthAct = new QAction(tr("Pen &Width"), this);
    penWidthAct->setEnabled(false);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    clearScreenAct = new QAction(tr("&Clear Screen"), this);
    connect(clearScreenAct, SIGNAL(triggered()),
            scribbleArea, SLOT(clearImage()));
}

void mainwindow::createMenus()
{
    saveAsMenu = new QMenu(tr("&Save As"), this);
    foreach (QAction *action, saveAsActs)
        saveAsMenu->addAction(action);

    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addAction(openAct2);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(input_datas);
    optionMenu->addAction(select_tumor);
    optionMenu->addAction(compute_algorithm);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
}

bool mainwindow::maybeSave()
{
    if (scribbleArea->isModified()) {
       QMessageBox::StandardButton ret;
       ret = QMessageBox::warning(this, tr("Scribble"),
                          tr("The image has been modified.\n"
                             "Do you want to save your changes?"),
                          QMessageBox::Save | QMessageBox::Discard
                          | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            return saveFile("png");
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

bool mainwindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(QString::fromLatin1(fileFormat.toUpper()))
                               .arg(QString::fromLatin1(fileFormat)));
    if (fileName.isEmpty()) {
        return false;
    } else {
        return scribbleArea->saveImage(fileName, fileFormat.constData());
    }
}
