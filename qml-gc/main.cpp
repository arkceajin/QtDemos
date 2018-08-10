#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QQmlContext>

class Control : public QObject{
    Q_OBJECT
public:
    using QObject::QObject;
    ~Control() { qDebug() << "deleting control"; }
};

class DomainManager: public QObject{
    Q_OBJECT
public:
    using QObject::QObject;
    Q_INVOKABLE Control* controlWriter(QString partition){
        Control *control = new Control;
        //QQmlEngine::setObjectOwnership(control, QQmlEngine::CppOwnership);
        return  control;
    }
signals:
    void collectGarbage();
};

DomainManager *example = nullptr;

static QObject *domain_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return example;
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    example = new DomainManager;

    qmlRegisterSingletonType<DomainManager>("my.pkg", 1, 0, "DomainManager", domain_provider);
    qmlRegisterUncreatableType<Control>("my.pkg", 1, 0, "Control", "Get it fresh from DomainManager");

    QQmlApplicationEngine engine;
    QObject::connect(example, &DomainManager::collectGarbage,
                     [&engine]() {
        engine.collectGarbage();
        qDebug("collectGarbage");
    });
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

#include "main.moc"
