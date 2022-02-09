#pragma once
#include <QDebug>
#include <qdir.h>
#include <QCoreApplication>
#include "FaceContext.h"
#include "PixelInContext.h"
#include "PixelOutContext.h"
#include "PixelIterator.h"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <glm/mat4x4.hpp>
#include "VertexInContext.h"
#include "VertexOutContext.h"
#include "FaceContext.h"
#include "Shader.h"
typedef OpenMesh::TriMesh_ArrayKernelT<OpenMesh::DefaultTraits> Mesh;

class MeshRenderer
{
public:
	Shader shader;
	Mesh mesh;
	MeshRenderer();
	void Render(glm::mat4 mvpMatrix, glm::mat4 screenMatrix);
	void LogMatrix(glm::mat4 matrix);
};

