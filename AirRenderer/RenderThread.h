#pragma once
#include <QtWidgets>
#include "Renderer.h"
#include "Configuration.h"
#include "glm/mat4x4.hpp"
#include "RenderItem.h"

class RenderThread : public QThread
{
	Q_OBJECT
public:

	QTimer* timer;
	void Run();
	void GetCameras(std::vector<RenderItem<GameObject>>& vector);
	void GetCamerasDFS(std::vector<RenderItem<GameObject>>& vector, GameObject* gameObject, mat4 parentMatrix);
	void GetMeshRenderers(std::vector<RenderItem<GameObject>>& vector);
	void GetMeshRenderersDFS(std::vector<RenderItem<GameObject>>& vector, GameObject* gameObject, glm::mat4 parentMatrix);
	void Display();
	void LogMatrix(glm::mat4 Matrix);
	RenderThread(QObject* parent);
	Renderer* renderer;
private slots:void Render();
};