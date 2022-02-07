#pragma once

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <glm/mat4x4.hpp>
#include "VertexContext.h"
#include "FaceContext.h"
typedef OpenMesh::TriMesh_ArrayKernelT<OpenMesh::DefaultTraits> Mesh;

class MeshRenderer
{
public:
	Mesh mesh;
	MeshRenderer();
	void Render(glm::mat4 mvpMatrix, glm::mat4 screenMatrix);
	void LogMatrix(glm::mat4 matrix);
};

