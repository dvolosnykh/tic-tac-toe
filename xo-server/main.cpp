#include "server.h"
#include "serverwidget.h"
#include <QtWidgets/QApplication>


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Server server;
    ServerWidget widget(&server);
    widget.show();

    return app.exec();
}
