#pragma once
#include <QtWidgets>
#include "include/core_object/Configuration.h"
#include "glm/mat4x4.hpp"
#include "include/thread/RenderItem.h"
#include <include/thread/RenderCommandBuffer.h>
#include <QWaitCondition>
#include <QMutex>
#include <include/thread/RenderCommandBuffer.h>
class RenderThread : public QThread
{
	Q_OBJECT
	void run() override;
public:
	//QTimer* timer;
	//void Run();
	//void GetCameras(std::vector<RenderItem<GameObject>>& vector);
	//void GetCamerasDFS(std::vector<RenderItem<GameObject>>& vector, GameObject* gameObject);
	//void GetMeshRenderers(std::vector<RenderItem<GameObject>>& vector);
	//void GetMeshRenderersDFS(std::vector<RenderItem<GameObject>>& vector, GameObject* gameObject, glm::mat4 parentMatrix);
	//void GetLights(std::vector<RenderItem<GameObject>>& vector);
	//void GetLightsDFS(std::vector<RenderItem<GameObject>>& vector, GameObject* gameObject, glm::mat4 parentMatrix);
	void SubmitCommandBuffer(std::shared_ptr<RenderCommandBuffer> RenderCommandBuffer);
	RenderThread(QObject* parent);
private:
	void Display();
	void Render(std::shared_ptr<RenderCommandBuffer> renderCommandBuffer);
	void Pipeline(MatrixContext* matrixContext, LightContext* lightContext, CameraContext* cameraContext, Mesh* mesh, std::shared_ptr<ShaderBase> shader);
	QMutex commandBufferMutex;
	QWaitCondition commandBufferAvailable;
	std::vector< std::shared_ptr<RenderCommandBuffer>> commandBufferList;
//private slots:void Render();
};