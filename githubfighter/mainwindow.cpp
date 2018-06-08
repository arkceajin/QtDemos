#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(800, 600);
    mView.setScene(&mScene);
    setCentralWidget(&mView);
    mScene.start();
}

MainWindow::~MainWindow()
{

}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e)
    mScene.setSceneRect(0, 0, mView.width(), mView.height());
}
