#pragma once
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include "include/shader/Shader.h"
#include "include/material/Material.h"
#include "include/component/Component.h"
#include "include/context/MatrixContext.h"
#include "include/context/LightContext.h"

class MeshRenderer: public Component
{
public:
	typedef OpenMesh::TriMesh_ArrayKernelT<OpenMesh::DefaultTraits> Mesh;
	Shader shader;
	Mesh mesh;
	Material material;
	MeshRenderer();
	MeshRenderer(std::string filePath);
	void Render(MatrixContext* matrixContext, LightContext* lightContext);
};

