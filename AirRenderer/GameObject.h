#pragma once
#include "ChildBrotherTree.h"
#include "Transform.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "Object.h"
#include "Component.h"
class GameObject : public ChildBrotherTree<GameObject>, public Object
{
public:
	std::string name;
	Transform transform;
	std::vector<Component*>* components;
	GameObject();
	GameObject(std::string name);
	void UpdateSelf(void* data);
	void UpdateSelfWithoutTransform(void* data);
	void CascadeUpdate(void* data);

	void AddComponent(Component* component);
	template<typename T>
	T* FindComponent(std::string typeName);
	template<typename T>
	T* RemoveComponent(std::string typeName);
	void DestoryComponent(std::string typeName);
};

template<typename T>
T* GameObject::FindComponent(std::string typeName)
{
	T* t = nullptr;
	for each (Component * component in *components)
	{
		if (component->typeName == typeName)
		{
			t = static_cast<T*>(component);
			break;
		}
	}
	return t;
}
template<typename T>
T* GameObject::RemoveComponent(std::string typeName)
{
	int index = -1;
	for (int i = 0; i < components->size(); i++)
	{
		if (components->operator[](i)->typeName == typeName)
		{
			index = i;
			break;
		}
	}
	if (index >= 0)
	{
		T* t = static_cast<T*>(components->operator[](index));
		components->erase(components->begin() + index);
		return t;
	}
	else
	{
		return nullptr;
	}
}
