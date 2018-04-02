#include <QApplication>
#include <ImageSlider.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    (new ImageSlider(QStringList()
                     <<":/img/img1.jpg"
                     <<":/img/img2.jpg"
                     <<":/img/img3.jpg"
                     <<":/img/img4.jpg"
                     <<":/img/img7.jpg"
                     <<":/img/img8.jpg"
                     <<":/img/img10.jpg"
                     <<":/img/img11.jpg"
                     <<":/img/img12.jpg"))->show();

    return a.exec();
}
