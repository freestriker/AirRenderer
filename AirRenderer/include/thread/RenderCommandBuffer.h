#pragma once
#include <include/core_object/Mesh.h>
#include <include/shader/ShaderBase.h>
#include <include/material/MaterialBase.h>
#include <include/context/CameraContext.h>
#include <include/component/camera/Camera.h>
#include <include/component/light/Light.h>
#include <memory>
class RenderCommandBuffer
{
public:
	class CameraRenderWrap
	{
	public:
		CameraContext cameraContext;
		std::vector<int> materialInstanceIndex;
		CameraRenderWrap(Camera& camera);
	};
	class MaterialRenderWrap
	{
	public:
		std::shared_ptr <MaterialBase> materialInstance;
		int meshInstanceIndex;
		int passIndex;
		glm::mat4 worldMatrix;
		MaterialRenderWrap(MaterialBase* materialInstance, int meshIndex, glm::mat4 worldMatrix, int passIndex);
	};

	std::vector< CameraRenderWrap> cameraRenderWraps;
	std::vector< MaterialRenderWrap> materialRenderWraps;
	std::vector<Mesh> meshInstances;
	std::vector<Light*> lightInstances;


	RenderCommandBuffer();

};
class RenderCommandBufferBuilder
{
	std::map<Camera*, int> cameraRenderWrapMap;
	std::map<std::string, int> meshMap;
	int curCameraRenderWrapIndex;

	std::vector< RenderCommandBuffer::CameraRenderWrap> cameraRenderWraps;
	std::vector< RenderCommandBuffer::MaterialRenderWrap> materialRenderWraps;
	std::vector<Mesh> meshInstances;
	std::vector<Light*> lightInstances;
public:
	RenderCommandBufferBuilder();
	std::shared_ptr< RenderCommandBuffer> BuildCommandBuffer();
	void SetCamera(Camera& camera);
	void AddLight(Light& light);
	void DrawMesh(Mesh& modelMesh, glm::mat4& worldMatrix, MaterialBase& material, int passIndex);

};