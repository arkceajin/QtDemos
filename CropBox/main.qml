import QtQuick 2.12
import QtQuick.Window 2.12
import "CropBox"

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Column{
        anchors.centerIn: parent
        Text {
            id: test
        }
        Image {
            source: "sample.jpg"
            CropBox {
                anchors.fill: parent
                onCropRectChanged: {
                    test.text = " x:" + cropRect.x +
                                " y:" + cropRect.y +
                                " w:" + cropRect.width +
                                " h:" + cropRect.height
                }
            }
        }
    }
}
