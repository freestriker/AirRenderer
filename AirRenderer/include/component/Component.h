#pragma once
#include "include/utils/Object.h"
#include <include/utils/CrossLinkedList.h>
class GameObject;
class Component: public Object, public CrossLinkedNode
{
public:
	GameObject* gameObject;
	Component(std::string typeName);
	virtual ~Component();
	virtual void UpdateSelf(void* data);
	static void Destory(Component* component);
	template<typename T>
	static T* FindComponent(CrossLinkedRowHead linkedComponents, std::string typeName);
	template<typename T>
	static void AddComponent(CrossLinkedRowHead linkedComponents, T* component);
	template<typename T>
	static T* RemoveComponent(CrossLinkedRowHead linkedComponents, std::string typeName);

};

template<typename T>
T* Component::FindComponent(CrossLinkedRowHead linkedComponents, std::string typeName)
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
void Component::AddComponent(CrossLinkedRowHead linkedComponents, T* component)
{
	Component* t = static_cast<Component*>(component);
	linkedComponents.AddNode(t);
}

template<typename T>
T* Component::RemoveComponent(CrossLinkedRowHead linkedComponents, std::string typeName)
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
