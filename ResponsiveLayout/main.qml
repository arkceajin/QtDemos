import QtQuick 2.6
import QtQuick.Window 2.2

Window {
    id: window
    visible: true
    width: 300; height: 500

    Row {
        anchors.fill: parent
        states: [
            State {
                when: window.width > 500
                PropertyChanges {
                    target: contacts; width: 300
                }
                PropertyChanges {
                    target: chat; width: window.width - contacts.width; visible: true
                }
            }
        ]
        Rectangle {
            id: contacts
            color: "lightblue"; border.width: 5; border.color: "white"
            width: parent.width; height: parent.height
        }
        Rectangle{
            id: chat
            color: "lightgray"; border.width: 5; border.color: "white"
            visible: false; height: parent.height
        }
    }
}
