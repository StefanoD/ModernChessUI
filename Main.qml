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
                    property variant colorArray: ["dimgrey", "floralwhite"]
                    signal boardRotated()

                    Repeater {
                        anchors.fill: parent
                        model: _board.squares

                        delegate: DropArea {
                            id: dropArea
                            width: board.squareSize
                            height: width

                            required property int index
                            required property string square

                            Rectangle {
                                property int row: Math.floor(index/8);
                                property int col: index % 8;
                                height: board.squareSize
                                width: board.squareSize
                                color: board.colorArray[(row+1+col) % 2]
                                Text {
                                    id: field
                                    text: square
                                }
                            }
                        }
                    }
                }

                Grid {
                    id: figures
                    objectName: "figures"
                    anchors.fill: parent
                    rows: 8
                    columns: 8

                    readonly property int squareSize: Math.min(width, height) / 8;

                    Repeater {
                        anchors.fill: parent
                        model: _board.squares

                        delegate: MouseArea {
                            required property int index
                            required property string square
                            required property string imageSource

                            height: figures.squareSize
                            width: figures.squareSize

                            property int row: Math.floor(index/8);
                            property int col: index % 8;

                            Drag.active: drag.active
                            Drag.hotSpot.x: width / 2
                            Drag.hotSpot.y: height / 2
                            drag.target: figurePlaceholder
                            Item {
                                id: figurePlaceholder

                                anchors.fill: parent
                                Image {
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
