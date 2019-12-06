import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 500
    height: 500
    ScrollView2 {
        id: scroll
        anchors.fill: parent
        Text {
            width: 1000
            height: 1000
            text: "ABC"
            font.pixelSize: 800
            Component.onCompleted: {
                scroll.scrollTo(Qt.Horizontal, 0.5)
                scroll.scrollTo(Qt.Vertical, 0.5)
            }
        }
    }
}
