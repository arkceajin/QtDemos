# QtDemos
This is the collection of Qt demos to solve the problem from `StackOverflow` or I faced.

### 1. Image Slider
From: https://stackoverflow.com/questions/49568486/preview-image-on-slider-of-videoplayer

### 2. Responsive Layout
From: https://stackoverflow.com/questions/49745226/create-a-responsive-ui-like-telegram-using-qt-quick-and-qml

### 3. Colorize Progress Bar
From: https://stackoverflow.com/questions/49867207/how-do-i-change-the-state-of-a-qprogressbar

### 4. Github Fighter(Shooting game)
From: https://stackoverflow.com/questions/50753042/qgraphicspixmapitem-collision-detect-no-match-for-operator-operand-type-is

### 5. QmlObjectList
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

### 6. QML Garbage Collection
From: https://stackoverflow.com/questions/51729672/what-are-the-rules-for-a-c-object-returned-from-a-q-invokable-to-be-owned-and

GC is not the smart pointer. When the object's reference count becomes zero, QML GC will destroy the object which returns from `Q_INVOKABLE` function except `CppOwnership`. But not immediately. GC.
