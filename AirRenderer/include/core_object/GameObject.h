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
	CrossLinkedRowHead linkedComponents;
	GameObject();
	virtual ~GameObject();
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
	for (CrossLinkedNodeRowItertor rowIter = linkedComponents.GetItertor(); rowIter.IsVaild(); ++rowIter)
	{
		Component* component = rowIter.Node<Component>();
		if (component->typeName == typeName)
		{
			return static_cast<T*>(component);
		}
	}
	return nullptr;
}
template<typename T>
void GameObject::AddComponent(T* component)
{
	Component* t = static_cast<Component*>(component);
	linkedComponents.AddNode(t);
	t->gameObject = this;
	t->UpdateSelf(this->parent);
}
template<typename T>
T* GameObject::RemoveComponent(std::string typeName)
{
	Component* target = nullptr;
	for (CrossLinkedNodeRowItertor rowIter = linkedComponents.GetItertor(); rowIter.IsVaild(); ++rowIter)
	{
		Component* component = rowIter.Node<Component>();
		if (component->typeName == typeName)
		{
			target = component;
			break;
		}
	}
	if (target)
	{
		linkedComponents.RemoveNode(target);
		return target;
	}
	else
	{
		return nullptr;
	}
}
