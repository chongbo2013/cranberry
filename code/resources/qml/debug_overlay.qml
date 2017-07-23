import QtQuick 2.7
import QtQuick.Controls 1.4
import kgl.cb.treemodel 1.0

Item {
    width: 320
    height: 200

    Rectangle {
        color: Qt.rgba(1, 1, 1, 0.7)
        radius: 10
        border.width: 2
        border.color: "white"
        anchors.fill: members
        anchors.margins: -10
    }

    TreeModel {
        id: treemodel
        Component.onCompleted: console.log(this.myint)
    }

    TreeView {
        id: members
        //model: treemodel.model
        backgroundVisible: false
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 20

        rowDelegate: Rectangle {
            // Sets the background color to transparent.
            color: "transparent"
        }

        itemDelegate: Item {
            // Alters the font of each item.
            Text {
                //font: "Monospace"
                //font.pixelSize: 12
                text: styleData.value
                color: styleData.color
                elide: styleData.elideMode
            }
        }
    }
}
