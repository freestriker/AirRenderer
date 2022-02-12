#pragma once
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Material.h"
#include "Component.h"

class MeshRenderer: public Component
{
public:
	typedef OpenMesh::TriMesh_ArrayKernelT<OpenMesh::DefaultTraits> Mesh;
	Shader shader;
	Mesh mesh;
	Material material;
	MeshRenderer();
	MeshRenderer(std::string filePath);
	void Render(glm::mat4 mvpMatrix, glm::mat4 screenMatrix);
};

