#pragma once

#include <QtCore/QObject>


QT_FORWARD_DECLARE_CLASS(QTcpSocket)


class Client : public QObject
{
    Q_OBJECT

public:
    Client();
    virtual ~Client();

public slots:
    void startSession(const QString& address, int port);
    void sendWhoFirst(bool meFirst);
    void placeMarker(int index);

signals:
    void connected();
    void error(const QString& message);
    void serverPlacedMarker(int index);
    void playerWon();
    void computerWon();
    void drawGame();

private:
    void receive();

private:
    QTcpSocket* const _connection;
};
