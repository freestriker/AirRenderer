#pragma once
#include "include/shader/Shader.h"
#include "include/material/Material.h"
#include "include/component/Component.h"
#include "include/context/MatrixContext.h"
#include "include/context/LightContext.h"
#include <include/thread/LoadThread.h>

class MeshRenderer: public Component
{
public:
	LoadThread::LoadCommand loadCommand;
	MaterialBase* material;
	MeshRenderer();
	MeshRenderer(std::string filePath);
	void Render(MatrixContext* matrixContext, LightContext* lightContext);
};

