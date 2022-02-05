#pragma once

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
typedef OpenMesh::TriMesh_ArrayKernelT<OpenMesh::DefaultTraits> Mesh;

class MeshRenderer
{
public:
	Mesh mesh;
	MeshRenderer();
};

