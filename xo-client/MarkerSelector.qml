import Marker 1.0
import "utils.js" as Utils
import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

Item {
    signal selectionMade(int myMarker, int opponentMarker)

    ColumnLayout {
        anchors.centerIn: parent

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Выберите предпочитаемый символ:")
        }

        ListView {
            id: markerList
            width: height * 2; height: 128
            orientation: ListView.Horizontal

            model: ListModel {
                ListElement { marker: Marker.Cross }
                ListElement { marker: Marker.Circle }
            }

            delegate: Image {
                width: height; height: markerList.height
                source: Utils.markerImage(marker, Marker)

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: markerList.currentIndex = index
                    onClicked: {
                        var opponentMarker = (marker === Marker.Cross ? Marker.Circle : Marker.Cross)
                        selectionMade(marker, opponentMarker)
                    }
                }
            }

            highlight: Rectangle {
                color: "lightsteelblue"
                radius: 5
            }
        }
    }
}
