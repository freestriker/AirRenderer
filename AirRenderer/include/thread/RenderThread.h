#pragma once
#include <QtWidgets>
#include "include/core_object/Configuration.h"
#include "glm/mat4x4.hpp"
#include "include/thread/RenderItem.h"

class RenderThread : public QThread
{
	Q_OBJECT
public:

	QTimer* timer;
	void Run();
	void GetCameras(std::vector<RenderItem<GameObject>>& vector);
	void GetCamerasDFS(std::vector<RenderItem<GameObject>>& vector, GameObject* gameObject);
	void GetMeshRenderers(std::vector<RenderItem<GameObject>>& vector);
	void GetMeshRenderersDFS(std::vector<RenderItem<GameObject>>& vector, GameObject* gameObject, glm::mat4 parentMatrix);
	void GetLights(std::vector<RenderItem<GameObject>>& vector);
	void GetLightsDFS(std::vector<RenderItem<GameObject>>& vector, GameObject* gameObject, glm::mat4 parentMatrix);
	void Display();
	RenderThread(QObject* parent);
	private slots:void Render();
};