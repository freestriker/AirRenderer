#pragma once
#include <QImage>
#include <QLabel>
#include "GameObject.h"
#include "glm/mat4x4.hpp"
#include "Drawer.h"
#include "Color.h"

typedef struct DisplayResolution
{
public:
	int width = 800;
	int height = 450;
} Resolution;

class Configuration
{
public:
	Resolution resolution;
	QImage canvas;
	QLabel* label;
	Buffer<Color>* colorBuffer;

	GameObject sceneObject;
	glm::mat4 GetScreenMatrix();
	void Display();
	Configuration();
};

extern Configuration configuration;
