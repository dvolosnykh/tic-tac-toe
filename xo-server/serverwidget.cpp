#include "server.h"
#include "serverwidget.h"
#include "ui_serverwidget.h"
#include <QtNetwork/QHostAddress>


ServerWidget::ServerWidget(Server* const server)
    : _server(server)
    , ui(new Ui::ServerWidget)
{
    ui->setupUi(this);

    connect(_server, &Server::message, this, &ServerWidget::logMessage);

    const bool started = _server->start();
    if (started) {
        ui->editAddress->setText(_server->address().toString());
        ui->editPort->setText(QString::number(_server->port()));
    } else {
        ui->editAddress->setText(tr("Ошибка"));
        ui->editPort->setText(tr("Ошибка"));
    }
}

ServerWidget::~ServerWidget()
{}

void ServerWidget::logMessage(const int sessionId, const QString& message)
{
    ui->textLog->appendPlainText(QStringLiteral("%1: %2").arg(sessionId).arg(message));
}
