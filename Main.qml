import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 1024
    height: 1024
    visible: true
    title: qsTr("Modern Chess")
    id: root
    readonly property int squareSize: Math.min(width, height) / 8;


    ListModel {
            id: figures

            ListElement { name: "a1"; imageSource: "/resources/white-rook.svg" }
            ListElement { name: "a8"; imageSource: "/resources/white-rook.svg" }
        }

    RowLayout {
        Rectangle {
            id: board
            Layout.alignment: Qt.AlignTop
            property variant colorArray: ["floralwhite", "dimgrey"]
            property bool flipped: false

            Repeater {
                model: 64

                delegate: Rectangle {
                    required property int index

                    property int row: Math.floor(index/8);
                    property int col: index % 8;
                    z: 0;
                    x: col * squareSize
                    y: (7 - row) * squareSize
                    height: squareSize
                    width: squareSize
                    color: board.colorArray[(row+1+col) % 2]
                    Text {
                        id: field
                        text: board.flipped ? String.fromCharCode('h'.charCodeAt(0) - col) + (8 - row) : String.fromCharCode('a'.charCodeAt(0) + col) + (row + 1)
                    }
                    Image {
                        id: figure
                        source: (field.text === "a1") ? "/resources/white-king.svg" : ""
                        antialiasing: true
                        anchors.fill: parent
                        anchors.margins: parent.width * 0.025
                    }

                    /*MouseArea {
                        anchors.fill: parent
                        anchors.margins: parent.width * 0.025
                        onClicked: {

                        }
                        onDragChanged: {
                            figure.x = mouseX
                            figure.y = mouseY
                        }
                    }*/
                }
            }
        }
        Button {
            Layout.alignment: Qt.AlignBottom
            text: "Rotate"
            onClicked: {
                board.flipped = !board.flipped
            }
        }
    }
}
