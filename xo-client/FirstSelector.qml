import Marker 1.0
import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

Item {
    signal selectionMade(bool meFirst)

    ColumnLayout {
        anchors.centerIn: parent

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Кто начнёт игру первым?")
        }

        ListView {
            id: markerList
            width: height * 2; height: 128
            orientation: ListView.Horizontal

            model: ListModel {
                ListElement { me: true }
                ListElement { me: false }
            }

            delegate: Image {
                width: height; height: markerList.height
                source: (me ? "human.svg" : "computer.svg")

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: markerList.currentIndex = index
                    onClicked: selectionMade(me)
                }
            }

            highlight: Rectangle {
                color: "lightsteelblue"
                radius: 5
            }
        }
    }
}
