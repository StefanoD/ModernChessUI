import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 1024
    height: 1024
    visible: true
    title: qsTr("Modern Chess")
    id: root

    color: "#888888";

    ColumnLayout {
        id: boardWithAllElements
        anchors.fill: parent
            Item {
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                Layout.fillHeight: true

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
                            DropArea {
                                id: dropArea

                                width: board.squareSize
                                height: width
                                 MouseArea {
                                     id: dragArea
                                     anchors.fill: parent
                                     drag.target: parent
                                    Image {
                                        id: figure
                                        z: 1
                                        source: imageSource
                                        antialiasing: true
                                        anchors.fill: parent
                                        anchors.margins: parent.width * 0.025
                                    }
                                }
                            }
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
