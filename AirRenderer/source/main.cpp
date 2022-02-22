#include "include/thread/AirRenderer.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>
#include <include/core_object/Global.h>
#include <include/thread/LoadThread.h>
#include <include/thread/RenderThread.h>
#include <include/utils/OrientedBoundingBox.h>
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    AirRenderer window = AirRenderer();
    window.show();
    
    OrientedBoundingBox oob = OrientedBoundingBox();
    glm::mat3 m = glm::mat3(
        2, 0, 0,
        0, 2, 0,
        0, 0, 1
    );
    glm::vec3 vs[3];
    float s[3];
    oob.jacobiSolver(m, s, vs);
    global.loadThread = new LoadThread(&window);
    global.renderThread = new RenderThread(&window);

    global.loadThread->start();
    global.renderThread->Run();


    return a.exec();
}