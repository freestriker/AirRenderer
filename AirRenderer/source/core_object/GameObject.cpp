#include "include/core_object/GameObject.h"

GameObject::GameObject():GameObject("New GameObject")
{

}
GameObject::~GameObject()
{
}
GameObject::GameObject(std::string name):Object("GameObject"), linkedComponents()
{
	this->transform = Transform();
	this->transform.gameObject = this;
	this->name = name;
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

