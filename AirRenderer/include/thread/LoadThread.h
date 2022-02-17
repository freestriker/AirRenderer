#pragma once
#include <qthread.h>
#include <queue>
#include <map>
#include <QWaitCondition>
#include <QMutex>

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
    ResourceWrap* LoadMesh(std::string path);
    ResourceWrap* LoadTexture(std::string path);
    void AddCommand(LoadThread::LoadCommand loadCommand);
    ResourceWrap* GetResourceWrap(std::string path);
    std::queue<LoadCommand> commandQueue;
    std::map<std::string, ResourceWrap*> resourecMap;

    QMutex commandQueueMutex;
    QWaitCondition commandAvailable; 
    QWaitCondition resourceAvailable; 
    QMutex resourecMapMutex;
signals:
    void resultReady(const QString&);
};