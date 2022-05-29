#include "mainwindow.h"
#include "QDir"
#include "QFileDialog"
#include "QAction"
#include "QColorDialog"
#include "QInputDialog"
#include "QMessageBox"
#include "QImageWriter"
#include "QMenu"
#include "QMenuBar"
#include "QStatusBar"
#include "QScrollBar"


MainWindow::MainWindow()
{
    resize(1900, 1400);
    paintingArea = new Paint;
    setCentralWidget(paintingArea);
    setMouseTracking(true);
    createActions();
    createMenus();
    this->setMouseTracking(true);

    label = new QLabel(this);
    QString s = "Current figure : ";
    s += paintingArea->getFigure();
    label->setText(s);
    statusBar()->addWidget(label);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(maybeSave())
        event->accept();
    else event->ignore();
}

void MainWindow::open()
{
    if(maybeSave()){
        QString fileName = QFileDialog::getOpenFileName(this, "Open File", QDir::currentPath());
        if(!fileName.isEmpty()){
            paintingArea->openImage(fileName);
        }
    }
}

void MainWindow::save()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void MainWindow::penColor()
{
    QColor newColor = QColorDialog::getColor(paintingArea->getPenColor());
    if(newColor.isValid()){
        paintingArea->setPenColor(newColor);
    }
}

void MainWindow::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, "Paint", "Select pen width : ", paintingArea->getPenWidth(), 1, 50, 1, &ok);
    if(ok){
        paintingArea->setPenWidth((newWidth));
    }
}

void MainWindow::renewStatusBar()
{
    QString s = "Current figure : ";
    s += paintingArea->getFigure();
    label->setText(s);
}

void MainWindow::createActions()
{
    openAct = new QAction("Open", this);
    openAct->setShortcut(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
    foreach(QByteArray format, QImageWriter::supportedImageFormats()){
        QString text = tr("%1...").arg(QString(format).toUpper());
        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(save()));
        saveAsActs.append(action);
    }
    exitAct = new QAction("Exit", this);
    exitAct->setShortcut(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    penColorAct = new QAction("Pen Color", this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));
    penWidthAct = new QAction("Pen Width", this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    clearScreenAct = new QAction("Clear Screen", this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, SIGNAL(triggered()), paintingArea, SLOT(clearImage()));

    chooseLineAct = new QAction("Line", this);
    connect(chooseLineAct, SIGNAL(triggered()), paintingArea, SLOT(chooseLine()));
    chooseCurveAct = new QAction("Curve", this);
    connect(chooseCurveAct, SIGNAL(triggered()), paintingArea, SLOT(chooseCurve()));
    chooseRectAct = new QAction("Rectangle", this);
    connect(chooseRectAct, SIGNAL(triggered()), paintingArea, SLOT(chooseRect()));
    chooseEllipseAct = new QAction("Ellipse", this);
    connect(chooseEllipseAct, SIGNAL(triggered()), paintingArea, SLOT(chooseEllipse()));

    connect(paintingArea, SIGNAL(figure()), this, SLOT(renewStatusBar()));
}

void MainWindow::createMenus()
{
    saveAsMenu = new QMenu("Save As", this);
    foreach(QAction *action, saveAsActs)
        saveAsMenu->addAction(action);
    fileMenu = new QMenu("File", this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionMenu = new QMenu("Options", this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);

    figuresMenu = new QMenu("Figures", this);
    figuresMenu->addAction(chooseLineAct);
    figuresMenu->addAction(chooseCurveAct);
    figuresMenu->addAction(chooseRectAct);
    figuresMenu->addAction(chooseEllipseAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addSeparator();
    menuBar()->addMenu(figuresMenu);
}

bool MainWindow::maybeSave()
{
    if(paintingArea->isModified()){
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, "Paint", "The image has been modified.\n" "Do you want to save your changes",
                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save){
            return saveFile("png");
        } else if (ret == QMessageBox::Cancel){
            return false;
        }
    }
    return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;
    QString fileName = QFileDialog::getSaveFileName(this, "Save As", initialPath,
                                                    tr("%1 Files (*) .%2;; AllFiles(*)").arg(QString::fromLatin1(fileFormat.toUpper()))
                                                    .arg(QString::fromLatin1(fileFormat)));
    if(fileName.isEmpty()){
        return false;
    } else {
        return paintingArea->saveImage(fileName, fileFormat.constData());
    }
}

