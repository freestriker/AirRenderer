#pragma once
#include <include/core_object/Mesh.h>
#include <include/shader/ShaderBase.h>
#include <include/material/MaterialBase.h>
#include <include/context/CameraContext.h>
#include <include/component/camera/Camera.h>
#include <include/component/light/Light.h>
class RenderCommandBuffer
{
public:
	class CameraRenderWrap
	{
	public:
		CameraContext cameraContext;
		std::vector<int> shaderIndex;
		CameraRenderWrap(Camera& camera)
		{
			cameraContext.needPerspectiveCorrection = camera.needPerspectiveCorrection;
			cameraContext.projectionMatrix = camera.ProjectionMatrix();
			cameraContext.worldMatrix = camera.WorldMatrix();
			shaderIndex = std::vector<int>();
		}
	};
	class MaterialRenderWrap
	{
	public:
		MaterialBase* material;
		int meshIndex;
		glm::mat4 worldMatrix;
		MaterialRenderWrap(MaterialBase* material, int meshIndex, glm::mat4 worldMatrix)
		{
			this->material = material;
			this->meshIndex = meshIndex;
			this->worldMatrix = worldMatrix;
		}
	};

	std::map<Camera*, int>* cameraRenderWrapMap;
	std::map<std::string, int>* meshMap;
	int curCameraRenderWrapIndex;
	std::vector< CameraRenderWrap> cameraRenderWrap;
	std::vector< MaterialRenderWrap> materialRenderWrap;
	std::vector<Mesh> mesh;
	std::vector<Light*> lights;
	void SetCamera(Camera& camera)
	{
		if (cameraRenderWrapMap->count(&camera))
		{
			curCameraRenderWrapIndex = cameraRenderWrapMap->operator[](&camera);
		}
		else
		{
			curCameraRenderWrapIndex = cameraRenderWrap.size();
			cameraRenderWrapMap->operator[](&camera) = curCameraRenderWrapIndex;
			cameraRenderWrap.push_back(CameraRenderWrap(camera));
		}
	}
	void AddLight(Light& light)
	{
		this->lights.push_back(light.Clone());
	}
	void DrawMesh(Mesh& modelMesh, glm::mat4& worldMatrix, MaterialBase& material)
	{
		int meshIndex;
		if (meshMap->count(modelMesh.loadCommand.path))
		{
			meshIndex = meshMap->operator[](modelMesh.loadCommand.path);
		}
		else
		{
			meshIndex = mesh.size();
			meshMap->operator[](modelMesh.loadCommand.path) = meshIndex;
			mesh.push_back(modelMesh);
		}

		cameraRenderWrap[curCameraRenderWrapIndex].shaderIndex.push_back(materialRenderWrap.size());

		materialRenderWrap.push_back(MaterialRenderWrap(material.Clone(), meshIndex, worldMatrix));
	}

	RenderCommandBuffer()
	{
		cameraRenderWrapMap = new std::map<Camera*, int>();
		meshMap = new std::map<std::string, int>();
		curCameraRenderWrapIndex = -1;

		cameraRenderWrap = std::vector< CameraRenderWrap>() ;
		materialRenderWrap = std::vector< MaterialRenderWrap>() ;
		mesh = std::vector<Mesh> ();

	}

	void SubmitCommandBuffer()
	{
		delete cameraRenderWrapMap;
		delete meshMap;
		curCameraRenderWrapIndex = -1;
	}
};