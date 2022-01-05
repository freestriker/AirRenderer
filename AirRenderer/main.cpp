#include "AirRenderer.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AirRenderer w;
    w.show();
    return a.exec();
}
