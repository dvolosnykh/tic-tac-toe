#include "client.h"
#include "../xo-server/commands.h"
#include <QtNetwork/QTcpSocket>


Client::Client()
    : _connection(new QTcpSocket)
{
    connect(_connection, &QTcpSocket::connected, this, &Client::connected);
    connect(_connection, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), [this]()
    {
        emit error(_connection->errorString());
    });
    connect(_connection, &QTcpSocket::readyRead, this, &Client::receive);
}

Client::~Client()
{
    _connection->close();
}

void Client::startSession(const QString& address, int port)
{
    _connection->connectToHost(address, port);
}

void Client::sendWhoFirst(const bool meFirst)
{
    const Command command = (meFirst ? Command::PlayerFirst : Command::ComputerFirst);
    _connection->write(reinterpret_cast<const char*>(&command), sizeof(command));
    _connection->flush();
}

void Client::placeMarker(const int index)
{
    _connection->write(reinterpret_cast<const char*>(&index), sizeof(index));
    _connection->flush();
}

void Client::receive()
{
    Command command;
    _connection->read(reinterpret_cast<char*>(&command), sizeof(command));

    switch (command) {
    case Command::PlayerWon:
        emit playerWon();
        break;

    case Command::ComputerWon:
        emit computerWon();
        break;

    case Command::DrawGame:
        emit drawGame();
        break;

    default:
        emit serverPlacedMarker(static_cast<int>(command));
    }
}
