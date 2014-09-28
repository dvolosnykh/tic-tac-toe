import QtQuick 2.3
import QtQuick.Controls 1.2

ApplicationWindow {
    title: "xo-client"
    visible: true
    width: 640; height: 480

    StackView {
        id: stack
        initialItem: connectionSettings
    }

    ConnectionSettings {
        id: connectionSettings
        visible: false
        onConnectionEstablished: stack.push(markerSelector)
    }
    MarkerSelector {
        id: markerSelector
        visible: false
        onSelectionMade: {
            board.myMarker = myMarker; board.opponentMarker = opponentMarker
            board.reset()
            stack.push(firstSelector)
        }
    }
    FirstSelector {
        id: firstSelector
        visible: false
        onSelectionMade: {
            client.sendWhoFirst(meFirst)
            stack.push(board)
        }
    }
    Board {
        id: board
        visible: false
        onGameEnded: stack.pop(markerSelector)
    }

    Connections {
        target: client
        onError: stack.pop(connectionSettings)
    }
}
