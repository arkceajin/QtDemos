import QtQuick 2.0
import QtQuick.Controls 2.4

Rectangle {
    id: root
    color: text.enabled ? "transparent" : "#353637"
    border.color: text.enabled ? "#21be2b" : "transparent"
    property int padding: 10
    property alias font: text.font

    MouseArea {
        anchors.fill: root
        onClicked: {
            text.forceActiveFocus()
        }
    }

    Flickable {
        id: flickable
        anchors.fill: parent
        anchors.leftMargin: root.padding
        anchors.rightMargin: anchors.leftMargin
        contentHeight: root.height
        contentWidth: text.contentWidth + text.cursorRectangle.width
        clip: true
        flickableDirection: Flickable.HorizontalFlick
        ScrollBar.horizontal: ScrollBar {
            id: hbar; active: true
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    // Because ScrollBar stack on the TextInput
                    // forceActiveFocus when click on the ScrollBar
                    text.forceActiveFocus()
                }
            }
        }
        TextInput{
            id: text
            width: Math.max(root.width - root.padding * 2, contentWidth)
            height: root.height
            autoScroll: false
            verticalAlignment: TextInput.AlignVCenter
            onCursorRectangleChanged: {
                // TODO: let the visibleArea follow the cursor movement
            }
        }
    }
}
