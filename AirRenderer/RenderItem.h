#pragma once
#include "glm/mat4x4.hpp"

template<typename T>
class RenderItem
{
public:
	T* item;
	glm::mat4 transformationMatrix;
	RenderItem(T* item, glm::mat4 transformationMatrix);
};
template<typename T>
RenderItem<T>::RenderItem(T* item, glm::mat4 transformationMatrix)
{
	this->item = item;
	this->transformationMatrix = transformationMatrix;
}

