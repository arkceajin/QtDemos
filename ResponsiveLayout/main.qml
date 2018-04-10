import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

Window {
    id: window
    visible: true
    readonly property int defaultWidth: 300
    readonly property int responsiveWidth: 500
    width: defaultWidth
    height: 500

    Row {
        anchors.fill: parent
        states: [
            State {
                when: window.width >= responsiveWidth // Change responsive layout base on the width
                // When the current width is bigger than responsiveWidth,
                // following properties will be changed.
                PropertyChanges { 
                    target: contacts; width: defaultWidth
                }
                PropertyChanges {
                    target: chat; width: window.width - contacts.width; visible: true
                }
                PropertyChanges {
                    target: showChat; visible: false
                }
            }
        ]
        Rectangle {
            id: contacts
            color: "lightblue"; border.width: 5; border.color: "white"
            width: parent.width; height: parent.height
            Button {
                id: showChat
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottomMargin: parent.border.width
                text: "Show chat"
                onClicked: window.width = responsiveWidth
            }
        }
        Rectangle{
            id: chat
            color: "lightgray"; border.width: 5; border.color: "white"
            visible: false; height: parent.height
            Button {
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottomMargin: parent.border.width
                text: "Hide chat"
                onClicked: window.width = defaultWidth
            }
        }
    }
}
