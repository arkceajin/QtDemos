import QtQuick 2.9
import QtQuick.Controls 2.2
import QtMultimedia 5.9

ApplicationWindow {
    visible: true
    width: 500; height: 500

    VideoOutput {
        id: videoOuput
        anchors.fill: parent
        source: opencvCam
    }
}
