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
	template<typename T>
	void RemoveAllComponent(std::vector<T*>& vector);
};

template<typename T>
T* GameObject::FindComponent(std::string typeName)
{
	return Component::FindComponent<T>(linkedComponents, typeName);
}
template<typename T>
void GameObject::AddComponent(T* component)
{
	Component::AddComponent<T>(linkedComponents, component);
	global.logicThread->AddComponent(component);

	Component* t = static_cast<Component*>(component);
	t->gameObject = this;
	t->UpdateSelf(this->parent);
	t->OnAddAsComponent();
}
template<typename T>
T* GameObject::RemoveComponent(std::string typeName)
{
	T* t = Component::RemoveComponent<T>(linkedComponents, typeName);
	Component* c = static_cast<Component*>(t);
	global.logicThread->RemoveComponent(c);

	c->gameObject = nullptr;
	return t;
}

template<typename T>
void GameObject::RemoveAllComponent(std::vector<T*>& vector)
{
	Component::RemoveAllComponent<T>(linkedComponents, vector);
}
