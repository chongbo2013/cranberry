import QtQuick 2.7
import QtQuick.Controls 1.4

Item {
    width: 320
    height: 240

    Rectangle {
        color: Qt.rgba(1, 1, 1, 0.9)
        radius: 10
        border.width: 2
        border.color: "white"
        anchors.fill: members
        anchors.margins: -10
    }

    TreeView {
        id: members
        objectName: "members"
        model: debugModel
        backgroundVisible: false
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 2

        // Each column gets half of the tree view.
        onWidthChanged: {
            firstcol.width = width / 2
            secondcol.width = width / 2
        }

        TableViewColumn {
            id: firstcol
            title: "Property name"
            role: "member"
            resizable: false
        }

        TableViewColumn {
            id: secondcol
            title: "Property value"
            role: "value"
            resizable: true
        }

        headerDelegate: Rectangle {
            // Alters the header styling.
            color: "transparent"
            height: 20
            anchors.topMargin: -20

            Text {
                x: styleData.value === "Property name" ? 5 : 0
                y: 2
                id: htext
                font: monospace
                color: "black"
                text: styleData.value
            }
        }

        rowDelegate: Rectangle {
            // Sets the background color to transparent.
            color: "transparent"
        }

        itemDelegate: Item {
            // Alters the font and text color of each item.
            Text {
                font: monospace
                color: "black"
                text: styleData.value === undefined ? "" : styleData.value
                elide: styleData.elideMode
            }
        }
    }
}
