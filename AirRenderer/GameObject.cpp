#include "GameObject.h"

GameObject::GameObject():GameObject("New GameObject")
{

}
GameObject::GameObject(std::string name):Object("GameObject")
{
	this->transform = Transform();
	this->transform.gameObject = this;
	this->name = name;
	components = new std::vector<Component*>();
}

void GameObject::UpdateSelf(void* data)
{
	transform.UpdateSelf(data);
	for each (Component* component in *components)
	{
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
void GameObject::UpdateSelfWithoutTransform(void* data)
{
	for each (Component * component in *components)
	{
		component->UpdateSelf(data);
	}
}
void GameObject::AddComponent(Component* component)
{
	components->push_back(component);
	component->gameObject = this;
	component->UpdateSelf(this->parent ? this->parent : nullptr);
}
void GameObject::DestoryComponent(std::string typeName)
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
		Component* c = components->operator[](index);
		components->erase(components->begin() + index);
		delete c;
	}
}


