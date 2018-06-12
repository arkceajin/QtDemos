#include "mainwindow.h"
#include <QVBoxLayout>
#include <QPushButton>

#define Start "Start"
#define Stop "Stop"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mView.setScene(&mScene);
    QPushButton* b = new QPushButton(Start);
    setCentralWidget(new QWidget(this));
    QVBoxLayout* v = new QVBoxLayout;
    v->addWidget(&mView);
    v->addWidget(b);
    centralWidget()->setLayout(v);

    connect(b, &QPushButton::clicked, [=]() {
        if(mScene.started()) {
            b->setText(Start);
            mScene.stop();
        } else {
            b->setText(Stop);
            mScene.start();
        }
    });

    resize(800, 600);
}

MainWindow::~MainWindow()
{

}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    QMainWindow::resizeEvent(e);
    mScene.setSceneRect(0, 0, mView.width(), mView.height());
}

