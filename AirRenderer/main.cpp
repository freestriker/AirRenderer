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

    //GameObject go = GameObject();
    //GameObject* c1 = go.AddChild();
    //GameObject* c2 = go.AddChild();
    //GameObject* c3 = go.AddChild();
    //GameObject* c4 = go.AddChild();
    //GameObject::ChildIterator si = go.GetStartChildIterator();
    //GameObject::ChildIterator ei = go.GetEndChildIterator();
    //int m = 0;
    //for (GameObject::ChildIterator i = go.GetStartChildIterator(); i != ei; i++)
    //{
    //    m++;
    //}
    //GameObject::BrotherIterator bsi = c4->GetStartBrotherIterator();
    //GameObject::BrotherIterator bei = c4->GetEndBrotherIterator();
    //int n = 0;
    //for (GameObject::BrotherIterator i = c4->GetStartBrotherIterator(); i != bei; i++)
    //{
    //    n++;
    //}

    RenderThread* rt = new RenderThread(&a);
    rt->Run();

    return a.exec();
}