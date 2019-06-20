import QtQuick 2.11
import QPropertyAnimation 1.0

Image {
    property alias duration: needleAnimation.duration
    property alias minAngle: needleAnimation.startValue
    property alias maxAngle: needleAnimation.endValue
    property int maxSpeed
    readonly property int angleRange: maxAngle - minAngle
    readonly property double rpmRatio: (needleAnimation.currentValue - minAngle) / angleRange   // rpm = rpmRatio * 8000
    readonly property double rpmThreshold: 6500 / 8000  // Show the red needle if exceeds the threshold(6500)
    readonly property int speed: rpmRatio * maxSpeed
    source: "file:assets/gauge.png"

    Text {
        id: speedText
        anchors.centerIn: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: speed + "\nkm/h"
        color: "#FFF"
        font.family: fontDejaVuSansMonoBold.name
        font.pixelSize: 60
    }

    Image {
        id: needle
        source: (rpmRatio > rpmThreshold)? "file:assets/red-needle.png": "file:assets/needle.png"
        x: (parent.sourceSize.width / 2) - (needle.width / 2)
        y: (parent.sourceSize.height / 2)
        transformOrigin: Item.Top
        rotation: minAngle
    }

    QPropertyAnimation {
        id: needleAnimation
        propertyName: "rotation"
        targetObject: needle
    }

    function increase() {
        if(needle.rotation == maxAngle)
            return;
        needleAnimation.direction = QPropertyAnimation.Forward
        if(needleAnimation.state == QPropertyAnimation.Paused)
            needleAnimation.resume();
        else
            needleAnimation.start();
    }

    function decrease() {
        if(needle.rotation == minAngle)
            return;
        needleAnimation.direction = QPropertyAnimation.Backward
        if(needleAnimation.state == QPropertyAnimation.Paused)
            needleAnimation.resume();
        else
            needleAnimation.start();
    }

    function stop() {
        if(needleAnimation.state != QPropertyAnimation.Stopped)
            needleAnimation.pause()
    }
}
