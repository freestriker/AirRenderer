#pragma once
#include <qthread.h>
#include <QWaitCondition>
#include <QMutex>
#include <include/thread/RenderItem.h>
#include <include/core_object/GameObject.h>
#include <include/component/Component.h>
class LogicThread : public QThread
{
    Q_OBJECT
    void run() override;
    std::map<std::string, CrossLinkedColHead*> componentListMap;
public:
    LogicThread(QObject* parent);

    template<typename T>
    void AddComponent(T* component);
    template<typename T>
    T* RemoveComponent(T* component);
    void Init();
};

template<typename T>
void LogicThread::AddComponent(T* component)
{
    Component* t = static_cast<Component*>(component);
    CrossLinkedColHead* head = nullptr;
    if (componentListMap.count(t->typeName))
    {
        head = componentListMap[t->typeName];
    }
    else
    {
        head = new CrossLinkedColHead();
        componentListMap[t->typeName] = head;
    }
    head->AddNode(t);
}

template<typename T>
T* LogicThread::RemoveComponent(T* component)
{
    Component* t = static_cast<Component*>(component);
    CrossLinkedColHead* head = componentListMap[t->typeName];
    head->RemoveNode(t);
    return static_cast<T*>(t);
}
