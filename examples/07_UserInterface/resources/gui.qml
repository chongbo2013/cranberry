import QtQuick 2.7

Item {
    width: 600
    height: 600

    Rectangle {
        color: Qt.rgba(1, 1, 1, 0.7)
        radius: 10
        border.width: 2
        border.color: "white"
        anchors.fill: label
        anchors.margins: -10
    }

    Text {
        id: label
        font.family: "Consolas"
        font.pixelSize: 16
        color: "black"
        wrapMode: Text.WordWrap
        text: "This is an illustration of how to embed a Qml UI in a cranberry window, using Qt Quick 2.0 and cran::GuiManager. Underneath, a hexagon with color rgb(183, 18, 95) is rendered."
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 20
    }
}
