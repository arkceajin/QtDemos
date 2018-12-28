import QtQuick 2.12

Rectangle {
    property alias xAxis: dragHandler.xAxis
    property alias yAxis: dragHandler.yAxis
    signal dragged(var point)

    width: spotRadius; height: spotRadius
    radius: spotRadius
    color: dragHandler.active? dragColor: spotColor
    DragHandler{
        id: dragHandler
        onTranslationChanged: {
            dragged(Qt.point(x, y));
        }
    }
}
