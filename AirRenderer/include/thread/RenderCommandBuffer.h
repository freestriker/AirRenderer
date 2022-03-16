#pragma once
#include <include/core_object/Mesh.h>
#include <include/context/CameraContext.h>
#include <include/component/camera/Camera.h>
#include <include/component/light/Light.h>
#include <memory>
#include <include/material/Material.h>
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
		std::shared_ptr <Material> materialInstance;
		int meshInstanceIndex;
		int passIndex;
		glm::mat4 worldMatrix;
		MaterialRenderWrap(Material* materialInstance, int meshIndex, glm::mat4 worldMatrix, int passIndex);
	};

	std::vector< CameraRenderWrap> cameraRenderWraps;
	std::vector< MaterialRenderWrap> materialRenderWraps;
	std::vector< std::shared_ptr <Mesh>> meshInstances;
	std::vector< std::shared_ptr <Light>> lightInstances;


	RenderCommandBuffer();
	~RenderCommandBuffer();

};
class RenderCommandBufferBuilder
{
	std::map<Camera*, int> cameraRenderWrapMap;
	std::map<std::string, int> meshMap;
	int curCameraRenderWrapIndex;

	std::vector< RenderCommandBuffer::CameraRenderWrap> cameraRenderWraps;
	std::vector< RenderCommandBuffer::MaterialRenderWrap> materialRenderWraps;
	std::vector< std::shared_ptr <Mesh>> meshInstances;
	std::vector< std::shared_ptr <Light>> lightInstances;
public:
	RenderCommandBufferBuilder();
	~RenderCommandBufferBuilder();
	std::shared_ptr< RenderCommandBuffer> BuildCommandBuffer();
	void SetCamera(Camera& camera);
	void AddLight(Light& light);
	void DrawMesh(Mesh& modelMesh, glm::mat4& worldMatrix, Material& material, int passIndex);

};