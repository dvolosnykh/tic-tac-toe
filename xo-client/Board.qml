import Marker 1.0
import "utils.js" as Utils
import QtQuick 2.3
import QtQuick.Dialogs 1.2

Item {
    readonly property int tilesCount: 3

    property int myMarker: Marker.None
    property int opponentMarker: Marker.None

    signal gameEnded

    Connections {
        target: client
        onServerPlacedMarker: {
            grid.enabled = true
            boardModel.set(index, { marker: opponentMarker })
        }
        onPlayerWon: {
            endMessage.text = qsTr("Вы победили!")
            endMessage.visible = true
        }
        onComputerWon: {
            endMessage.text = qsTr("Победил компьютер.")
            endMessage.visible = true
        }
        onDrawGame: {
            endMessage.text = qsTr("Ничья.")
            endMessage.visible = true
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "white"
    }
    Image {
        source: "board.svg"
        anchors.fill: parent
    }
    Grid {
        id: grid
        anchors.fill: parent
        columns: tilesCount

        Repeater {
            model: boardModel
            Item {
                width: parent.width / tilesCount
                height: parent.height / tilesCount

                Image {
                    anchors.fill: parent
                    source: Utils.markerImage(marker, Marker)
                }
                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        if (boardModel.get(index).marker === Marker.None) {
                            boardModel.set(index, { marker: myMarker })
                            grid.enabled = false
                            client.placeMarker(index)
                        }
                    }
                }
            }
        }
    }

    ListModel {
        id: boardModel
        Component.onCompleted: {
            for (var i = 0; i < tilesCount * tilesCount; ++i) {
                append({ marker: Marker.None })
            }
        }
    }

    MessageDialog {
        id: endMessage
        title: qsTr("Конец игры")
        onAccepted: gameEnded()
    }

    function reset() {
        for (var i = 0; i < tilesCount * tilesCount; ++i) {
            boardModel.set(i, { marker: Marker.None })
        }
        grid.enabled = true
    }
}
