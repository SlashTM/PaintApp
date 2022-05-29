#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "QLabel"
#include "QScrollBar"
#include "paint.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void open();
    void save();
    void penColor();
    void penWidth();

public slots:
    void renewStatusBar();

private:
    void createActions();
    void createMenus();
    bool maybeSave();
    bool saveFile(const QByteArray &fileFormat);
    QLabel *statusLabel;
    Paint *paintingArea;
    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QMenu *figuresMenu;
    QList <QAction *> saveAsActs;
    QScrollBar *scrollbar;
    QLabel *label;
    QAction *openAct;
    QAction *exitAct;
    QAction *penColorAct;
    QAction *penWidthAct;
    QAction *clearScreenAct;
    QAction *chooseLineAct;
    QAction *chooseCurveAct;
    QAction *chooseRectAct;
    QAction *chooseEllipseAct;
};
#endif // MAINWINDOW_H

