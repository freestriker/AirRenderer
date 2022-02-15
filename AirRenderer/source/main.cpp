#include "include/thread/AirRenderer.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "include/thread/RenderThread.h"
#include "include/core_object/GameObject.h"
#include <json.hpp>
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    AirRenderer window = AirRenderer();
    window.show();
    RenderThread rt = RenderThread(&window);
    rt.Run();

    return a.exec();
}