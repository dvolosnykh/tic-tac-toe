#pragma once

#include <QtCore/QObject>

QT_FORWARD_DECLARE_CLASS(QHostAddress)
QT_FORWARD_DECLARE_CLASS(QTcpServer)


class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server();
    virtual ~Server();

    bool start();
    void stop();

    int port() const;
    QHostAddress address() const;
    QString error() const;

signals:
    void message(int sessionId, const QString& text);

private:
    void startSession();

private:
    QScopedPointer<QTcpServer> _tcpServer;
};
