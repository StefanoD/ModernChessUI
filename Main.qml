import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 1024
    height: 1024
    visible: true
    title: qsTr("Modern Chess")
    id: root


    RowLayout {
        id: boardWithAllElements
        anchors.fill: parent
        width: parent.width
        height: parent.height


        Rectangle {
            id: board
            objectName: "board"
            width: parent.width
            height: parent.height
            Layout.alignment: Qt.AlignTop

            readonly property int squareSize: Math.min(parent.width, parent.height) / 8;
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
                    z: 0;
                    x: col * board.squareSize
                    y: (7 - row) * board.squareSize
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
}
