#pragma once
#include "Light.h"
#include <vector>
class LightContext
{
public:
	std::vector<Light*> lights;
	glm::vec3 cameraPosition;
};

