#include "AirRenderer.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "Renderer.h"
#include "Configuration.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AirRenderer w;
    w.show();
    Renderer renderer = Renderer();
    renderer.Render();
    renderer.Display();
    return a.exec();
}