#pragma once

#include "commands.h"
#include <QtCore/QObject>


class GameEngine;
QT_FORWARD_DECLARE_CLASS(QTcpSocket)


class Session : public QObject
{
    Q_OBJECT

public:
    explicit Session(QTcpSocket* connection, QObject* parent);
    virtual ~Session();

    int id() const;

signals:
    void message(int id, const QString& text);

private:
    void handleRequest();

    void startGame();
    bool playerTurn();
    void computerTurn();
    bool isGameEnded();

    Command pendingCommand();
    void sendCommand(Command command);

    void socketDisconnected();

private:
    const int _id;
    QTcpSocket* const _connection;
    QScopedPointer<GameEngine> _game;
};
