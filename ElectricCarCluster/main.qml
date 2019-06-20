import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.2

Window {
    visible: true
    width: 1920
    height: 720
    flags: Qt.FramelessWindowHint

    FontLoader {
        id: fontDejaVuSansMonoBold
        source: "file:assets/DejaVuSansMono-Bold.ttf"
    }

    Image {
        source: "file:assets/background.png"
    }

    Gauge {
        id: gauge
        anchors.centerIn: parent
        duration: 8000
        minAngle: 45
        maxAngle: 315
        maxSpeed: 200
        focus: true
        Keys.onPressed: {
            if(!event.isAutoRepeat) {
                if(event.key === Qt.Key_Up) {
                    gauge.increase();
                } else if (event.key === Qt.Key_Down) {
                    gauge.decrease();
                }
            }
        }
        Keys.onReleased: {
            if(!event.isAutoRepeat && (event.key === Qt.Key_Up || event.key === Qt.Key_Down)) {
                gauge.stop();
            }
        }
    }
}
