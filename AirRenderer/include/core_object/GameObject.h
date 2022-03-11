#pragma once
#include "include/utils/ChildBrotherTree.h"
#include "include/component/transform/Transform.h"
#include "include/component/camera/Camera.h"
#include "include/component/renderer/MeshRenderer.h"
#include "include/utils/Object.h"
#include "include/component/Component.h"
class GameObject : public ChildBrotherTree<GameObject>, public Object
{
public:
	std::string name;
	Transform transform;
	std::vector<Component*> components;
	GameObject();
	~GameObject();
	GameObject(std::string name);
	void UpdateSelf(void* data);
	void UpdateSelfWithoutTransform(void* data);
	void CascadeUpdate(void* data);
	void OnAddedAsChild(void* data);
	template<typename T>
	T* FindComponent(std::string typeName);
	template<typename T>
	void AddComponent(T* component);
	template<typename T>
	T* RemoveComponent(std::string typeName);
	static void Destory(GameObject* gameObject);
	static void DestorySelf(GameObject* gameObject);
};

template<typename T>
T* GameObject::FindComponent(std::string typeName)
{
	T* t = nullptr;
	for each (Component * component in components)
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
void GameObject::AddComponent(T* component)
{
	Component* t = static_cast<Component*>(component);
	components.push_back(t);
	t->gameObject = this;
	t->UpdateSelf(this->parent);
}
template<typename T>
T* GameObject::RemoveComponent(std::string typeName)
{
	int index = -1;
	for (int i = 0; i < components.size(); i++)
	{
		if (components.operator[](i)->typeName == typeName)
		{
			index = i;
			break;
		}
	}
	if (index >= 0)
	{
		Component* t = components.operator[](index);
		components.erase(components->begin() + index);
		t->gameObject = nullptr;
		
		return static_cast<T*>(t);
	}
	else
	{
		return nullptr;
	}
}
