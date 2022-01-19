#include "RenderThread.h"

void RenderThread::Run()
{
    Render();
    timer->setInterval(900);
    connect(timer, SIGNAL(timeout()), this, SLOT(Render()));
    timer->start();
    this->exec();
}

void RenderThread::Render()
{
    renderer->Render();
    renderer->Display();
}

RenderThread::RenderThread(QObject* parent):QThread(parent)
{
    timer = new QTimer(this);
    renderer = new Renderer();
}
