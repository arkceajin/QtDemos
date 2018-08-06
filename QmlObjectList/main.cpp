#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "qmlobjectlist.h"

class Human : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(int age MEMBER age)
public:
    Q_INVOKABLE Human(QString name = QString(),
                      int age = 0) : QObject(),
        name(name),
        age(age)
    { }
private:
    QString name;
    int     age;
};

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QmlObjectList* list = QmlObjectList::create<Human>();
    list->append(new Human("A", 12));
    list->append(new Human("B", 13));

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("myModel", QVariant::fromValue(list));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

#include "main.moc"
