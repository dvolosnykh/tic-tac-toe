import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

Item {
    signal connectionEstablished

    ColumnLayout {
        anchors.centerIn: parent

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Параметры соединения:")
        }
        RowLayout {
            TextField {
                id: address
                placeholderText: qsTr("адрес")
                Layout.fillWidth: true
                focus: true
            }
            Text { text: ":" }
            TextField {
                id: port
                placeholderText: qsTr("порт")
                validator: IntValidator { bottom: 0 }
            }
        }
        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Подключиться")
            isDefault: true
            onClicked: {
                errorMessage.clear()
                busy.visible = true
                client.startSession(address.text, Number(port.text))
            }
        }
    }

    BusyIndicator {
        id: busy
        visible: false
        anchors.centerIn: parent
        running: true
        onVisibleChanged: parent.enabled = !visible
    }

    Rectangle {
        id: errorMessage

        visible: false
        anchors.top: parent.top
        width: parent.width; height: errorText.height + 20
        border.color: "red"
        color: Qt.rgba(1, 0, 0, 0.2)

        Text {
            id: errorText
            anchors.centerIn: parent
            wrapMode: Text.WordWrap

            onTextChanged: parent.visible = (text !== "")
        }

        function clear() {
            errorText.text = ""
        }
    }

    Connections {
        target: client
        onConnected: {
            busy.visible = false
            connectionEstablished()
        }
        onError: {
            busy.visible = false
            errorText.text = message
        }
    }
}
