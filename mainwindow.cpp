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

void mainwindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open File"), QDir::currentPath());
        if (!fileName.isEmpty())
        {
            scribbleArea->openImage(fileName);
            input_datas->setEnabled(true);
            select_liver->setEnabled(true);
            select_tumor->setEnabled(true);
            penWidthAct->setEnabled(true);
        }
    }
}

void mainwindow::save()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void mainwindow::enterDatas()
{
    bool ok=false;
    QString volume_entered= QInputDialog::getText(this,"volume of the liver", "Enter the volume of the liver remaining at the end", QLineEdit::Normal, QString(), &ok);
    if (ok && !volume_entered.isEmpty())
    {
        ok=false;
        QString margin_entered= QInputDialog::getText(this,"Margin", "Enter the margin", QLineEdit::Normal, QString(), &ok);
        if(ok && !volume_entered.isEmpty())
        {
            volume=volume_entered;
            margin=margin_entered;
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

void mainwindow::selectLiver()
{
    scribbleArea->setImageLoaded(true);
    scribbleArea->setPenColor(Qt::red);
}

void mainwindow::selectTumor()
{
    scribbleArea->setImageLoaded(true);
    scribbleArea->setPenColor(Qt::green);
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
    openAct = new QAction(tr("&Open"), this);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

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

    select_liver=new QAction(tr("&Select Liver"), this);
    select_liver->setEnabled(false);
    connect(select_liver, SIGNAL(triggered()), this, SLOT(selectLiver()));

    select_tumor=new QAction(tr("&Select Tumor"), this);
    select_tumor->setEnabled(false);
    connect(select_tumor, SIGNAL(triggered()), this, SLOT(selectTumor()));

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
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(input_datas);
    optionMenu->addAction(select_liver);
    optionMenu->addAction(select_tumor);
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