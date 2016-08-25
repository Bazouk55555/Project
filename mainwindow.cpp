#include <QtWidgets>
#include <iostream>
#include <string>
#include "mainwindow.h"
#include "compute_volume.h"
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

void mainwindow::openFolderDicom()
{
    if (maybeSave()) {
        QString directory = QFileDialog::getExistingDirectory(this);
        if (!directory.isEmpty())
        {
            scribbleArea->openFolderDicom(directory);
            directory_dicom=directory.toStdString();
            input_datas->setEnabled(true);
            select_tumor->setEnabled(true);
            select_liver->setEnabled(true);
            penWidthAct->setEnabled(true);
            display_dicom_in_3d->setEnabled(true);
            segmentation->setEnabled(true);
        }
    }
}

//To open a folder with JPG images segmented
void mainwindow::openFolder()
{
    if (maybeSave()) {
        QString directory = QFileDialog::getExistingDirectory(this);
        if (!directory.isEmpty())
        {
            scribbleArea->openFolder(directory);
            input_datas->setEnabled(true);
            select_tumor->setEnabled(true);
            select_liver->setEnabled(true);
            penWidthAct->setEnabled(true);
            display_segmentation_in_3d->setEnabled(true);
        }
    }
}

//To open a an image file
void mainwindow::openFile()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
        {
            scribbleArea->openFile(fileName);
            input_datas->setEnabled(true);
            select_tumor->setEnabled(true);
            select_liver->setEnabled(true);
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

void mainwindow::displayIn3D()
{
    std::string command="ipython C:/Users/User/Desktop/testpy.py ";
    scribbleArea->clearImage();
    command.append(directory_dicom);
    system(command.c_str());
}

void mainwindow::displayJPGIn3D()
{
    std::string command="ipython C:/Users/User/Desktop/testpy2.py ";
    scribbleArea->clearImage();
    command.append(directory_segmentation.toStdString());
    system(command.c_str());
}

void mainwindow::segment()
{
    std::string command="ipython C:/Users/User/Desktop/project.py ";
    scribbleArea->clearImage();
    QString directory = QFileDialog::getExistingDirectory(this);
    directory_segmentation=directory;
    command.append(directory_dicom);
    command.append(" ");
    command.append(directory_segmentation.toStdString());
    std::cout<<command<<" lol!!!!!"<<std::endl;
    system(command.c_str());
    scribbleArea->openFolder(directory_segmentation);
    display_segmentation_in_3d->setEnabled(true);
}

//To enter the datas margin and volume expected
void mainwindow::enterDatas()
{
    bool ok=false;
    double volume_entered= QInputDialog::getDouble(this,"Volume of the liver", "Enter the volume of the liver remaining at the end", 0,0,1,2, &ok);
    if (ok)
    {
        ok=false;
        double margin_entered= QInputDialog::getDouble(this,"Margin", "Enter the margin", 0,0,2,2, &ok);
        if(ok)
        {
            volume=volume_entered;
            margin=margin_entered;
            std::cout<<"The volume of the margin is : "<<margiumLiver(scribbleArea->getImage(),margin)<<std::endl;
            compute_algorithm1->setEnabled(true);
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

void mainwindow::computeAlgorithm1()
{
    double pourcentage_liver=1-scribbleArea->computeAlgorithm1(margin);
    if(pourcentage_liver>volume)
    {
        std::string message="The volume remaining is ";
        message.append(std::to_string(pourcentage_liver));
        message.append(". A direct removal of the tuumor from the liver can be maid");
        QMessageBox::information(this, "Result", message.c_str());
        return;
    }
    QMessageBox::information(this, "Result", "Click on the option : Choose vein intersection to continue");
    technique2->setEnabled(true);
}

void mainwindow::computeAlgorithm2()
{
    bool ok=false;
    int part_of_liver=-1;
    while(part_of_liver!=0&&part_of_liver!=1){
        part_of_liver= QInputDialog::getInt(this,"Part of the liver", "Choose the part of the liver which must increase (1 for the low part, 0 for the high part)", 0,0,1,0, &ok);
        if((part_of_liver!=0&&part_of_liver!=1)&&ok)
        {
            QMessageBox::warning(this,"wrong number","The integer entered must be 0 or 1");
        }
    }
    if(ok)
    {
        double pourcentage_liver=1-scribbleArea->computeAlgorithm2(margin,part_of_liver);
        if(pourcentage_liver>volume)
        {
            std::string message="The volume remaining is ";
            message.append(std::to_string(pourcentage_liver));
            message.append(". A PVE can be maid");
            QMessageBox::information(this, "Result", message.c_str());
            return;
        }
    }
}

void mainwindow::selectTumor()
{
    scribbleArea->setImageLoaded(true);
    scribbleArea->setPenColor(Qt::blue);
}

void mainwindow::selectLiver()
{
    scribbleArea->setImageLoaded(true);
    scribbleArea->setPenColor(Qt::green);
}

void mainwindow::separationLiver()
{
    penWidthAct->setEnabled(false);
    scribbleArea->setPenWidth(1);
    scribbleArea->setImageLoaded(true);
    scribbleArea->setPenColor(Qt::red);
    compute_algorithm2->setEnabled(true);
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
    openDicom = new QAction(tr("&Open Dicom Folder"), this);
    connect(openDicom, SIGNAL(triggered()), this, SLOT(openFolderDicom()));

    openAct = new QAction(tr("&Open Jpg segmented images Folder"), this);
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

    display_dicom_in_3d = new QAction(tr("&Display non segmented images in 3d"), this);
    display_dicom_in_3d->setEnabled(false);
    connect(display_dicom_in_3d, SIGNAL(triggered()), this, SLOT(displayIn3D()));

    display_segmentation_in_3d = new QAction(tr("&Display segmented images in 3d"), this);
    display_segmentation_in_3d->setEnabled(false);
    connect(display_segmentation_in_3d, SIGNAL(triggered()), this, SLOT(displayJPGIn3D()));

    segmentation= new QAction(tr("&Segmentation"), this);
    segmentation->setEnabled(false);
    connect(segmentation, SIGNAL(triggered()), this, SLOT(segment()));

    input_datas= new QAction(tr("&Input Data"), this);
    input_datas->setEnabled(false);
    connect(input_datas, SIGNAL(triggered()), this, SLOT(enterDatas()));

    select_tumor=new QAction(tr("&Select Tumor"), this);
    select_tumor->setEnabled(false);
    connect(select_tumor, SIGNAL(triggered()), this, SLOT(selectTumor()));

    select_liver=new QAction(tr("&Select Liver"), this);
    select_liver->setEnabled(false);
    connect(select_liver, SIGNAL(triggered()), this, SLOT(selectLiver()));

    compute_algorithm1=new QAction(tr("&Compute direct removal of the tumor"), this);
    compute_algorithm1->setEnabled(false);
    connect(compute_algorithm1, SIGNAL(triggered()), this, SLOT(computeAlgorithm1()));

    compute_algorithm2=new QAction(tr("&Compute PVE"), this);
    compute_algorithm2->setEnabled(false);
    connect(compute_algorithm2, SIGNAL(triggered()), this, SLOT(computeAlgorithm2()));

    technique2=new QAction(tr("&Chose intersection vein points"), this);
    technique2->setEnabled(false);
    connect(technique2, SIGNAL(triggered()), this, SLOT(separationLiver()));

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
    fileMenu->addAction(openDicom);
    fileMenu->addAction(openAct);
    fileMenu->addAction(openAct2);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(display_dicom_in_3d);
    optionMenu->addAction(display_segmentation_in_3d);
    optionMenu->addAction(segmentation);
    optionMenu->addAction(input_datas);
    optionMenu->addAction(select_tumor);
    optionMenu->addAction(select_liver);
    optionMenu->addAction(compute_algorithm1);
    optionMenu->addAction(technique2);
    optionMenu->addAction(compute_algorithm2);
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
