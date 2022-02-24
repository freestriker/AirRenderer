#pragma once
#include <qthread.h>
#include <QWaitCondition>
#include <QMutex>
#include <include/thread/RenderItem.h>
#include <include/core_object/GameObject.h>
class LogicThread : public QThread
{
    Q_OBJECT
    void run() override;
public:
    LogicThread(QObject* parent);

    void GetCameras(std::vector<RenderItem<GameObject>>& vector);

    void GetCamerasDFS(std::vector<RenderItem<GameObject>>& vector, GameObject* gameObject);

    void GetMeshRenderers(std::vector<RenderItem<GameObject>>& vector);

    void GetMeshRenderersDFS(std::vector<RenderItem<GameObject>>& vector, GameObject* gameObject, glm::mat4 parentMatrix);

    void GetLights(std::vector<RenderItem<GameObject>>& vector);

    void GetLightsDFS(std::vector<RenderItem<GameObject>>& vector, GameObject* gameObject, glm::mat4 parentMatrix);

};