import QtQuick
import QtQml.Models
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: boardWithAllElements
    width: parent.width
    height: parent.height


    Grid {
        id: board
        objectName: "board"
        anchors.fill: parent
        rows: 8
        columns: 8

        readonly property int squareSize: Math.min(width, height) / 8;
        property variant colorArray: ["floralwhite", "dimgrey"]
        signal boardRotated()

        Repeater {
            anchors.fill: parent
            model: _board.squares

            delegate: Rectangle {
                required property int index
                required property string square
                required property string imageSource

                property int row: Math.floor(index/8);
                property int col: index % 8;
                height: board.squareSize
                width: board.squareSize
                color: board.colorArray[(row+1+col) % 2]
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

    Button {
        id: buttonRotateBoard
        Layout.alignment: Qt.AlignBottom
        text: "Rotate"
        onClicked: {
            board.boardRotated()
        }
    }
}
