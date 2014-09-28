#include "marker.h"
#include <QtQml>


void Marker::registerForQml()
{
    qmlRegisterType<Marker>("Marker", 1, 0, "Marker");
}
