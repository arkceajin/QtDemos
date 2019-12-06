import QtQuick 2.0
import QtQuick.Controls 2.4

ScrollView {
    id: root
    property ScrollBar hScrollBar: ScrollBar.horizontal
    property ScrollBar vScrollBar: ScrollBar.vertical

    /**
     * @param type [Qt.Horizontal, Qt.Vertical]
     * @param ratio 1.0 to 1.0
     */
    function scrollTo(type, ratio) {
        var scrollFunc = function (bar, ratio) {
            bar.setPosition(ratio - bar.size/2)
        }
        switch(type) {
        case Qt.Horizontal:
            scrollFunc(root.hScrollBar, ratio)
            break;
        case Qt.Vertical:
            scrollFunc(root.vScrollBar, ratio)
            break;
        }
    }
}
