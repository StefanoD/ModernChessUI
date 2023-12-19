import QtQuick
import QtQml.Models

Item {
    readonly property int squareSize: Math.min(parent.width, parent.height) / 8;
    property variant colorArray: ["floralwhite", "dimgrey"]
    property bool flipped: false

    Text {
        text: "Hello!"
    }

    ListView {
        anchors.fill: parent
        model: boardModel

        delegate: Rectangle {
            required property int index
            required property string square
            required property string imageSource

            property int row: Math.floor(index/8);
            property int col: index % 8;
            z: 0;
            x: col * squareSize
            y: (7 - row) * squareSize
            height: squareSize
            width: squareSize
            color: root.colorArray[(row+1+col) % 2]
            Text {
                id: field
                text: square
            }
            Image {
                id: figure
                source: imageSource
                antialiasing: true
                anchors.fill: parent
                anchors.margins: parent.width * 0.025
            }
        }
    }
}
