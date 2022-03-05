#pragma once
#include "include/shader/Shader.h"
#include "include/material/Material.h"
#include "include/component/Component.h"
#include "include/context/MatrixContext.h"
#include "include/context/LightContext.h"
#include <include/thread/LoadThread.h>
#include <include/context/CameraContext.h>
#include <include/core_object/Mesh.h>
#include <include/thread/RenderCommandBuffer.h>
class MeshRenderer: public Component
{
public:
	//LoadThread::LoadCommand loadCommand;
	Mesh mesh;
	MaterialBase* material;
	MeshRenderer();
	MeshRenderer(std::string filePath);
	void Render(RenderCommandBufferBuilder& commandBuilder);
};

