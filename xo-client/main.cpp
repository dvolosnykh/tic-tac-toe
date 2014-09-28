#include "client.h"
#include "marker.h"
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtWidgets/QApplication>


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
#ifdef Q_OS_MAC
    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath() + QStringLiteral("/../PlugIns"));
#endif
    QQmlApplicationEngine engine;
    Client client;
    engine.rootContext()->setContextProperty(QStringLiteral("client"), &client);
    Marker::registerForQml();
    engine.load(QUrl(QStringLiteral("qrc:/ui/MainWindow.qml")));
    return app.exec();
}
