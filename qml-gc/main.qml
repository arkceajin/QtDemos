import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import my.pkg 1.0

Window
{
    visible: true
    height: 640
    width: 480

    Component {
        id: test
        Item {
            property Control ctrl: DomainManager.controlWriter("test")
        }
    }

    Component.onCompleted: {
            var c = test.createObject()
            console.log("Control created")
            c.destroy()
            DomainManager.collectGarbage()
            console.log("Window onCompleted")
    }
}
