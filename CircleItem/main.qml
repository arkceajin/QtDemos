import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import CircleItem 1.0

Window {
    visible: true
    width: 600
    height: 480
    title: qsTr("Hello World")

    CircleItem {
        id: circle
        property real value: 0
        width: 390
        height: 390
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 13
        anchors.topMargin: 46
        angle: 160 * value / 8
        startAngle: 340
        clockwise: true
        color: "red"
        lineWidth: 20
        radius: 191
        vertex: 360
        Behavior on angle {
            NumberAnimation{
                duration: 1000
            }
        }
    }

    Timer {
        running: true
        repeat: true
        interval: 1000
        onTriggered: {
            circle.angle = circle.angle > 100? 0: 160
        }
    }
}
