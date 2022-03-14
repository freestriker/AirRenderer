#include "include/thread/AirRenderer.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>
#include <include/core_object/Global.h>
#include <include/thread/LoadThread.h>
#include <include/thread/RenderThread.h>
#include <include/utils/OrientedBoundingBox.h>
#include <include/thread/LogicThread.h>
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    AirRenderer window = AirRenderer();
    window.show();

    global.loadThread = new LoadThread(&window);
    global.renderThread = new RenderThread(&window);
    global.logicThread = new LogicThread(&window);

    global.loadThread->start();
    global.renderThread->start();

    global.logicThread->Init();
    global.logicThread->start();
    return a.exec();
}