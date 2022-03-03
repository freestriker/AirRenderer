#pragma once
#include <qthread.h>
#include <queue>
#include <map>
#include <QWaitCondition>
#include <QMutex>
#include <include/utils/OrientedBoundingBox.h>

class LoadThread : public QThread
{
    Q_OBJECT
    void run() override;
public:
    enum class LoadOption
    {
        LOAD,
        UNLOAD
    };
    enum class ProcessOptions
    {
        MESH,
        TEXTURE
    };
    class LoadCommand
    {
    public:
        LoadOption loadOption;
        ProcessOptions processOption;
        std::string path;
    };
    class ResourceWrap
    {
    public:
        void* resource;
        int referenceCount;
    };
    class MeshWrap
    {
    public:
        ModelMesh* modelMesh;
        OrientedBoundingBox* orientedBoundingBox;
    };
    LoadCommand Load(std::string path, ProcessOptions processOption);
    LoadCommand Load(LoadCommand& resourceKey);
    void Unload(LoadCommand& resourceKey);
    template <typename T>
    T* GetResource(LoadCommand& resourceKey);
    LoadThread(QObject* parent);
private:
    std::vector<LoadCommand> command;
    std::map<std::string, ResourceWrap*> resourecMap;

    ResourceWrap* LoadMesh(std::string path);
    ResourceWrap* LoadTexture(std::string path);

    QMutex commandQueueMutex;
    QWaitCondition commandAvailable; 
    QWaitCondition resourceAvailable; 
    QMutex resourecMapMutex;
};

template <typename T>
T* LoadThread::GetResource(LoadCommand& loadCommand)
{
    QMutexLocker lock(&resourecMapMutex);
    while (!resourecMap.count(loadCommand.path))
    {
        resourceAvailable.wait(&resourecMapMutex);
    }
    return static_cast<T*>(resourecMap[loadCommand.path]->resource);
}
