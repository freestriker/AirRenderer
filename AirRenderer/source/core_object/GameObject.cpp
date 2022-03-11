#include "include/core_object/GameObject.h"

GameObject::GameObject():GameObject("New GameObject")
{

}
GameObject::~GameObject()
{
}
GameObject::GameObject(std::string name):Object("GameObject")
{
	this->transform = Transform();
	this->transform.gameObject = this;
	this->name = name;
	components = std::vector<Component*>();
	this->linkedComponents = CrossLinkedRowHead();
	this->linkedComponents.Init();
}

void GameObject::UpdateSelf(void* data)
{
	transform.UpdateSelf(data);
	for (CrossLinkedNodeRowItertor rowIter = linkedComponents.GetItertor(); rowIter.IsVaild(); ++rowIter)
	{
		Component* component = rowIter.Node<Component>();
		component->UpdateSelf(data);

	}
}
void GameObject::CascadeUpdate(void* data)
{
	for (GameObject::ChildIterator startIterator = GetStartChildIterator(), endIterator = GetEndChildIterator(); startIterator != endIterator; startIterator++)
	{
		(*startIterator)->UpdateSelf(this);
	}
}
void GameObject::OnAddedAsChild(void* data)
{
	UpdateSelf(data);
	CascadeUpdate(data);
}
void GameObject::UpdateSelfWithoutTransform(void* data)
{
	for (CrossLinkedNodeRowItertor rowIter = linkedComponents.GetItertor(); rowIter.IsVaild(); ++rowIter)
	{
		Component* component = rowIter.Node<Component>();
		component->UpdateSelf(data);
	}
}

void GameObject::Destory(GameObject* gameObject)
{
	gameObject->RemoveSelf();
	std::vector<GameObject*> vector = std::vector<GameObject*>();
	gameObject->GetAllChildren(vector);
	for (int i = 0; i < vector.size(); i++)
	{
		DestorySelf(vector[i]);
	}
	DestorySelf(gameObject);
}
void GameObject::DestorySelf(GameObject* gameObject)
{
	for (int i = 0; i < gameObject->components.size(); i++)
	{
		Component::Destory(gameObject->components[i]);
	}
	gameObject->components.clear();	
	delete gameObject;
}


