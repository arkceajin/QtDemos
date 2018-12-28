import QtQuick 2.12

Item {
    readonly property color coverColor: "#ccc"
    readonly property double coverOpacity: 0.6
    readonly property color spotColor: "#555"
    readonly property color dragColor: "#6c2"
    readonly property int spotRadius: 10
    readonly property int spotOffset: spotRadius / -2
    readonly property int minArea: 50
    readonly property alias cropRect: cover.cropRect

    Rectangle {
        id: cover
        color: coverColor
        opacity: coverOpacity
        anchors.fill: parent
        property rect cropRect: Qt.rect(topLeft.x - spotOffset,
                                          topLeft.y - spotOffset,
                                          bottomRight.x - topLeft.x,
                                          bottomRight.y - topLeft.y)

        layer.enabled: true
        layer.effect: CoverShader {
            sourceRect: cover.cropRect
            xStep: 1 / cover.width
            yStep: 1 / cover.height
        }
    }

    DragSpot {
        id: topLeft
        x: spotOffset; y: spotOffset
        xAxis.minimum: spotOffset
        yAxis.minimum: spotOffset
        xAxis.maximum: bottomRight.x - minArea
        yAxis.maximum: bottomRight.y - minArea
        onDragged: {
            bottomLeft.x = point.x
            topRight.y = point.y
        }
    }

    DragSpot {
        id: topRight
        x: cover.width + spotOffset; y: spotOffset
        xAxis.minimum: topLeft.x + minArea
        yAxis.minimum: spotOffset
        xAxis.maximum: cover.width + spotOffset
        yAxis.maximum: bottomRight.y - minArea
        onDragged: {
            bottomRight.x = point.x
            topLeft.y = point.y
        }
    }

    DragSpot {
        id: bottomRight
        x: cover.width + spotOffset; y: cover.height + spotOffset
        xAxis.minimum: topLeft.x + minArea
        yAxis.minimum: topLeft.y + minArea
        xAxis.maximum: cover.width + spotOffset
        yAxis.maximum: cover.height + spotOffset
        onDragged: {
            topRight.x = point.x
            bottomLeft.y = point.y
        }
    }

    DragSpot {
        id: bottomLeft
        x: spotOffset; y: cover.height + spotOffset
        xAxis.minimum: spotOffset
        yAxis.minimum: topLeft.y + minArea
        xAxis.maximum: bottomRight.x - minArea
        yAxis.maximum: cover.height + spotOffset
        onDragged: {
            topLeft.x = point.x
            bottomRight.y = point.y
        }
    }
}
