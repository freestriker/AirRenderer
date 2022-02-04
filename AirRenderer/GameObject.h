#pragma once
#include "ChildBrotherTree.h"
#include "Transform.h"
class GameObject : public ChildBrotherTree<GameObject>
{
public:
	Transform transform;
	GameObject();
};

