#include "include/thread/AirRenderer.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "include/thread/RenderThread.h"
#include "include/core_object/GameObject.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    AirRenderer w;
    w.show();
    RenderThread* rt = new RenderThread(&a);
    rt->Run();

    return a.exec();
}