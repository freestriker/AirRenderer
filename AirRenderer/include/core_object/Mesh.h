#pragma once
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <include/thread/LoadThread.h>
#include <include/core_object/Global.h>
#include <include/utils/Object.h>
struct Traits : public OpenMesh::DefaultTraits
{
    VertexTraits
    {
    public :
        Point tangent;
    };
    FaceTraits
    {
        Point faceNormal;
    };
};
class ModelMesh : public OpenMesh::TriMesh_ArrayKernelT<Traits>
{

};
class Mesh:public Object
{
public:
    ModelMesh* modelMesh;
    OrientedBoundingBox* boundingBox;
    LoadThread::LoadCommand loadCommand;
    Mesh(std::string filePath): Object("Mesh")
    {
        this->modelMesh = nullptr;
        this->boundingBox = nullptr;
        this->loadCommand = global.loadThread->Load(filePath, LoadThread::ProcessOptions::MESH);
    }
    Mesh():Object("Mesh"), loadCommand()
    {
        modelMesh = nullptr;
        boundingBox = nullptr;
    }
    ~Mesh()
    {
        typeName = "DestoriedMesh";
        this->modelMesh = nullptr;
        this->boundingBox = nullptr;
        global.loadThread->Unload(loadCommand);
    }
    Mesh* Clone()
    {
        return new Mesh(loadCommand.path);
    }

    ModelMesh* GetModelMesh()
    {
        if (this->modelMesh == nullptr)
        {
            WaitForLoad();
        }
        return this->modelMesh;
    }
    void WaitForLoad()
    {
        if (loadCommand.valid)
        {
            LoadThread::MeshWrap* mw = global.loadThread->GetResource<LoadThread::MeshWrap>(loadCommand);
            this->modelMesh = mw->modelMesh;
            this->boundingBox = mw->orientedBoundingBox;
        }
    }
};