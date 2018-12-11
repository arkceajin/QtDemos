### Concolusion of QML Garbage collection:

QML GC is not the smart pointer. When the object's reference count becomes zero, QML GC will destroy the object which returns from `Q_INVOKABLE` function except `CppOwnership`. But not immediately. 
