#include "AirRenderer.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "Renderer.h"
#include "Configuration.h"
#include "RenderThread.h"
#include "GameObject.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AirRenderer w;
    w.show();

    RenderThread* rt = new RenderThread(&a);
    rt->Run();

    return a.exec();
}