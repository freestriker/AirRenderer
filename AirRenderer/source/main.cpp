#include "include/thread/AirRenderer.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>
#include <include/core_object/Global.h>
#include <include/thread/LoadThread.h>
#include <include/thread/RenderThread.h>
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    AirRenderer window = AirRenderer();
    window.show();
    
    global.loadThread = new LoadThread(&window);
    global.renderThread = new RenderThread(&window);


    global.loadThread->start();
    global.renderThread->Run();


    return a.exec();
}