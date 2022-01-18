#include "RenderThread.h"
#include "Renderer.h"

void RenderThread::Run()
{
    Render();
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(Render()));
    timer->start();
    this->exec();
}

void RenderThread::Render()
{
    Renderer renderer = Renderer();
    renderer.Render();
    renderer.Display();
}

RenderThread::RenderThread(QObject* parent):QThread(parent)
{
    timer = new QTimer(this);
}
