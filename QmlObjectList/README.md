I found that exposes a list of the data structure from C++ to QML almost appears all my programs using `QAbstractListModel`. So for convenience, I made the reusable class `QmlObjectList` which contains a list of smart pointers of `QObject`. Using just like the `QVector`.

For example the data structure like below:

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

Using the `list` just same as a combination of `QVector` & `QAbstractListModel`

    QmlObjectList* list = QmlObjectList::create<Human>();
    list->append(new Human("A", 12));
    list->append(new Human("B", 13));
