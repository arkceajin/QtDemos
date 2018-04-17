#include "ColorizeProgressBar.h"
#include <QApplication>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget w;
    QVBoxLayout l;
    w.setLayout(&l);

    QProgressBar* progress = new QProgressBar;
    progress->setValue(10);
    l.addWidget(progress);

    progress = new ColorizeProgressBar;
    progress->setValue(10);
    l.addWidget(progress);

    w.show();

    return a.exec();
}
