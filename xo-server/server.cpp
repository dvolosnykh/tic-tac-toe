#include "server.h"
#include "session.h"
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QTcpServer>


Server::Server()
    : _tcpServer(new QTcpServer)
{}

Server::~Server()
{
    stop();
}

bool Server::start()
{
    if (_tcpServer->isListening()) return true;

    const bool success = _tcpServer->listen(QHostAddress::AnyIPv4);
    if (success) {
        connect(_tcpServer.data(), &QTcpServer::newConnection, this, &Server::startSession);
    }

    return success;
}

void Server::stop()
{
    if (_tcpServer->isListening()) {
        _tcpServer->close();
    }
}

int Server::port() const
{
    return _tcpServer->serverPort();
}

QHostAddress Server::address() const
{
    foreach (const QHostAddress& address, QNetworkInterface::allAddresses()) {
        if (address != QHostAddress::LocalHost && address != QHostAddress::LocalHostIPv6 && address.toIPv4Address() != 0) {
            return address;
        }
    }

    return QHostAddress::LocalHost;
}

QString Server::error() const
{
    return _tcpServer->errorString();
}

void Server::startSession()
{
    auto* const session = new Session(_tcpServer->nextPendingConnection(), this);
    emit message(session->id(), tr("Начало сеанса"));
    connect(session, &Session::message, this, &Server::message);
}
