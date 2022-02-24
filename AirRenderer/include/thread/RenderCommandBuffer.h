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
		CameraRenderWrap(Camera& camera)
		{
			cameraContext.needPerspectiveCorrection = camera.needPerspectiveCorrection;
			cameraContext.projectionMatrix = camera.ProjectionMatrix();
			cameraContext.worldMatrix = camera.WorldMatrix();
			materialInstanceIndex = std::vector<int>();
		}
	};
	class MaterialRenderWrap
	{
	public:
		std::shared_ptr <MaterialBase> materialInstance;
		int meshInstanceIndex;
		glm::mat4 worldMatrix;
		MaterialRenderWrap(MaterialBase* materialInstance, int meshIndex, glm::mat4 worldMatrix)
		{
			this->materialInstance = std::shared_ptr <MaterialBase>(materialInstance);
			this->meshInstanceIndex = meshIndex;
			this->worldMatrix = worldMatrix;
		}
	};

	std::vector< CameraRenderWrap> cameraRenderWraps;
	std::vector< MaterialRenderWrap> materialRenderWraps;
	std::vector<Mesh> meshInstances;
	std::vector<Light*> lightInstances;


	RenderCommandBuffer()
	{
		cameraRenderWraps = std::vector< CameraRenderWrap>() ;
		materialRenderWraps = std::vector< MaterialRenderWrap>() ;
		meshInstances = std::vector<Mesh> ();

	}
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
	RenderCommandBufferBuilder()
	{
		cameraRenderWrapMap = std::map<Camera*, int>();
		meshMap = std::map<std::string, int>();
		curCameraRenderWrapIndex = -1;

		cameraRenderWraps = std::vector< RenderCommandBuffer::CameraRenderWrap>();
		materialRenderWraps = std::vector< RenderCommandBuffer::MaterialRenderWrap>();
		meshInstances = std::vector<Mesh>();
	}
	std::shared_ptr< RenderCommandBuffer> BuildCommandBuffer()
	{
		RenderCommandBuffer* rcb = new RenderCommandBuffer();
		rcb->cameraRenderWraps = this->cameraRenderWraps;
		rcb->materialRenderWraps = this->materialRenderWraps;
		rcb->meshInstances = this->meshInstances;
		rcb->lightInstances = this->lightInstances;
		return std::shared_ptr< RenderCommandBuffer>(rcb);
	}
	void SetCamera(Camera& camera)
	{
		if (cameraRenderWrapMap.count(&camera))
		{
			curCameraRenderWrapIndex = cameraRenderWrapMap.operator[](&camera);
		}
		else
		{
			curCameraRenderWrapIndex = cameraRenderWraps.size();
			cameraRenderWrapMap.operator[](&camera) = curCameraRenderWrapIndex;
			cameraRenderWraps.push_back(RenderCommandBuffer::CameraRenderWrap(camera));
		}
	}
	void AddLight(Light& light)
	{
		this->lightInstances.push_back(light.Clone());
	}
	void DrawMesh(Mesh& modelMesh, glm::mat4& worldMatrix, MaterialBase& material)
	{
		int meshIndex;
		if (meshMap.count(modelMesh.loadCommand.path))
		{
			meshIndex = meshMap.operator[](modelMesh.loadCommand.path);
		}
		else
		{
			meshIndex = meshInstances.size();
			meshMap.operator[](modelMesh.loadCommand.path) = meshIndex;
			meshInstances.push_back(modelMesh);
		}

		cameraRenderWraps[curCameraRenderWrapIndex].materialInstanceIndex.push_back(materialRenderWraps.size());

		materialRenderWraps.push_back(RenderCommandBuffer::MaterialRenderWrap(material.Clone(), meshIndex, worldMatrix));
	}

};