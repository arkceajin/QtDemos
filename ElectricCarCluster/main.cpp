#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QPropertyAnimation>
#include <QImage>
#include <QFileInfo>

#define GreenNeedleImg  "assets/needle.png"
#define RedNeedleImg    "assets/red-needle.png"
/**
 * @brief generate the red needle image if it not exists
 */
void prepareImage() {
    if(QFileInfo::exists(RedNeedleImg))
        return;
    QImage needle(GreenNeedleImg);
    QColor color;
    for(int y = 0; y < needle.height(); y++) {
        for(int x= 0; x < needle.width(); x++) {
            color = needle.pixelColor(x, y);
            color.setRgb(color.green(), color.red(), color.blue(), color.alpha());
            needle.setPixelColor(x, y, color);
        }
    }
    needle.save(RedNeedleImg);
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    prepareImage();
    qmlRegisterType<QPropertyAnimation>("QPropertyAnimation", 1, 0, "QPropertyAnimation");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
