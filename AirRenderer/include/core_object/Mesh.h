#pragma once
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <include/thread/LoadThread.h>
#include <include/core_object/Global.h>

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
class Mesh
{
private:
    ModelMesh* modelMesh;
    OrientedBoundingBox* boundingBox;
public:
    LoadThread::LoadCommand loadCommand;
    Mesh(std::string filePath)
    {
        this->loadCommand = global.loadThread->Load(filePath, LoadThread::ProcessOptions::MESH);
        this->modelMesh = nullptr;
    }
    Mesh():Mesh("../../Resources/Model/DefaultMesh.ply")
    {

    }

    ModelMesh* GetModelMesh()
    {
        if (this->modelMesh == nullptr)
        {
            LoadThread::MeshWrap* mw = global.loadThread->GetResource<LoadThread::MeshWrap>(loadCommand);
            this->modelMesh = mw->modelMesh;
            this->boundingBox = mw->orientedBoundingBox;
        }
        return this->modelMesh;
    }
};