#pragma once
#include "ChildBrotherTree.h"
#include "Transform.h"
#include "Camera.h"
#include "MeshRenderer.h"
class GameObject : public ChildBrotherTree<GameObject>
{
public:
	Transform transform;
	Camera camera;
	MeshRenderer meshRenderer;
	GameObject();
};

