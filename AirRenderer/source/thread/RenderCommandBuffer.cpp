#include <include/thread/RenderCommandBuffer.h>
RenderCommandBuffer::CameraRenderWrap::CameraRenderWrap(Camera& camera)
{
	cameraContext.needPerspectiveCorrection = camera.needPerspectiveCorrection;
	cameraContext.projectionMatrix = camera.ProjectionMatrix();
	cameraContext.worldMatrix = camera.WorldMatrix();
	cameraContext.aspectRatio = camera.aspectRatio;
	cameraContext.nearFlat = camera.nearFlat;
	cameraContext.farFlat = camera.farFlat;
	glm::vec4 clipPlanes[6] = { glm::vec4(1, 0, 0, 1), glm::vec4(-1, 0, 0, 1), glm::vec4(0, 1, 0, 1), glm::vec4(0, -1, 0, 1), glm::vec4(0, 0, 1, -camera.nearFlat / camera.farFlat), glm::vec4(0, 0, -1, 1) };
	cameraContext.primitiveCliper = PrimitiveCliper(clipPlanes, 6);
	materialInstanceIndex = std::vector<int>();
}
RenderCommandBuffer::MaterialRenderWrap::MaterialRenderWrap(MaterialBase* materialInstance, int meshIndex, glm::mat4 worldMatrix, int passIndex)
{
	this->materialInstance = std::shared_ptr <MaterialBase>(materialInstance);
	this->meshInstanceIndex = meshIndex;
	this->worldMatrix = worldMatrix;
	this->passIndex = passIndex;
}
RenderCommandBuffer::RenderCommandBuffer()
{
	cameraRenderWraps = std::vector< CameraRenderWrap>();
	materialRenderWraps = std::vector< MaterialRenderWrap>();
	meshInstances = std::vector<Mesh>();

}
RenderCommandBufferBuilder::RenderCommandBufferBuilder()
{
	cameraRenderWrapMap = std::map<Camera*, int>();
	meshMap = std::map<std::string, int>();
	curCameraRenderWrapIndex = -1;

	cameraRenderWraps = std::vector< RenderCommandBuffer::CameraRenderWrap>();
	materialRenderWraps = std::vector< RenderCommandBuffer::MaterialRenderWrap>();
	meshInstances = std::vector<Mesh>();
}
std::shared_ptr< RenderCommandBuffer> RenderCommandBufferBuilder::BuildCommandBuffer()
{
	RenderCommandBuffer* rcb = new RenderCommandBuffer();
	rcb->cameraRenderWraps = this->cameraRenderWraps;
	rcb->materialRenderWraps = this->materialRenderWraps;
	rcb->meshInstances = this->meshInstances;
	rcb->lightInstances = this->lightInstances;
	return std::shared_ptr< RenderCommandBuffer>(rcb);
}
void RenderCommandBufferBuilder::SetCamera(Camera& camera)
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
void RenderCommandBufferBuilder::AddLight(Light& light)
{
	this->lightInstances.push_back(light.Clone());
}
void RenderCommandBufferBuilder::DrawMesh(Mesh& modelMesh, glm::mat4& worldMatrix, MaterialBase& material, int passIndex)
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

	materialRenderWraps.push_back(RenderCommandBuffer::MaterialRenderWrap(material.Clone(), meshIndex, worldMatrix, passIndex));
}

