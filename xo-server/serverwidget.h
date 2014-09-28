#pragma once

#include <QtWidgets/QWidget>


namespace Ui {
class ServerWidget;
}
class Server;


class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServerWidget(Server* server);
    virtual ~ServerWidget();

private:
    void logMessage(int sessionId, const QString& message);

private:
    Server* const _server;
    QScopedPointer<Ui::ServerWidget> ui;
};
