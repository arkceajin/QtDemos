import QtQuick 2.10
import QtQuick.Window 2.3

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    ListView {
        anchors.fill: parent
        model: myModel
        delegate: Text{
            text: name + ", " + age
        }
        Component.onCompleted: {
            myModel.removeLast()
            myModel.append({"name": "C", "age": 22});
        }
    }
}
