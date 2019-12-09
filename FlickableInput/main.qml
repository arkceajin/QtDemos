import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.4

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    FlickableInput {
        width: 200
        height: 40
        anchors.centerIn: parent
    }
}
