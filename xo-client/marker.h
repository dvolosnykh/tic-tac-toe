#pragma once

#include <QtCore/QObject>

class Marker : public QObject
{
    Q_OBJECT
    Q_ENUMS(Type)

public:
    enum Type { None, Cross, Circle };

    static void registerForQml();
};
