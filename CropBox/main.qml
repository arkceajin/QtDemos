import QtQuick 2.12
import QtQuick.Window 2.12
import "CropBox"

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Image {
        anchors.centerIn: parent
        source: "sample.jpg"

        CropBox {
            anchors.fill: parent
        }
    }
}
