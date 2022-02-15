#pragma once
#include <QImage>
#include <QLabel>
#include "include/core_object/GameObject.h"
#include "glm/mat4x4.hpp"
#include "include/utils/Drawer.h"
#include "include/utils/Color.h"

typedef struct DisplayResolution
{
public:
	int width;
	int height;
	DisplayResolution(int width, int height)
	{
		this->width = width;
		this->height = height;
	}
	DisplayResolution():DisplayResolution(800, 450)
	{
	}
} Resolution;

class Configuration
{
public:
	Resolution resolution;
	QImage canvas;
	QLabel* label;
	Buffer<Color>* colorBuffer;
	Buffer<float>* depthBuffer;

	GameObject sceneObject = GameObject("Root GameObject");
	glm::mat4 GetScreenMatrix();
	Configuration();
};

extern Configuration configuration;
