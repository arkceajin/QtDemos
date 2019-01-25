import QtQuick 2.12

Item {
    id: cropBox
    property color coverColor: "#ccc"
    property double coverOpacity: 0.6
    property color spotColor: "#555"
    property color dragColor: "#6c2"
    property int spotRadius: 10
    property int spotOffset: spotRadius / -2
    property int minArea: 50
    property alias rectDraggable: dragRect.enabled
    readonly property alias cropRect: cover.cropRect

    /**
      * @breif set the cropRect
      * @param r new cropRect
      */
    function setRect(r) {
        topLeft.x = r.x + spotOffset
        topLeft.y = r.y + spotOffset

        topRight.x = r.x + r.width + spotOffset
        topRight.y = r.y + spotOffset

        bottomLeft.x = r.x + spotOffset
        bottomLeft.y = r.y + r.height + spotOffset

        bottomRight.x = r.x + r.width + spotOffset
        bottomRight.y = r.y + r.height + spotOffset
    }

    Rectangle {
        id: cover
        color: coverColor
        opacity: coverOpacity
        anchors.fill: parent
        property rect cropRect: Qt.rect(topLeft.x - spotOffset, topLeft.y - spotOffset,
                                        bottomRight.x - topLeft.x, bottomRight.y - topLeft.y)

        layer.enabled: true
        layer.effect: CoverShader {
            sourceRect: cover.cropRect
            xStep: 1 / cover.width
            yStep: 1 / cover.height
        }
    }

    Item {
        id: dragRect
        x: cropRect.x
        y: cropRect.y
        width: cropRect.width
        height: cropRect.height
        DragHandler{
            xAxis.minimum: 0
            yAxis.minimum: 0
            xAxis.maximum: cropBox.width - cropRect.width
            yAxis.maximum: cropBox.height - cropRect.height
            onTranslationChanged: {
                setRect(Qt.rect(parent.x, parent.y, cropRect.width, cropRect.height))
            }
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
