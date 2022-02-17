#pragma once
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

struct Traits : public OpenMesh::DefaultTraits
{
    VertexTraits
    {
    public :
        Point tangent;
    };
};
typedef OpenMesh::TriMesh_ArrayKernelT<Traits> Mesh;
