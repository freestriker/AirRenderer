#pragma once
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "include/utils/Color.h"
class VertexOutContext
{
public:
	glm::vec4 data[8]{ glm::vec4(0, 0, 0, 0) };
	float depth;
	glm::vec3 screenPosition;

	static VertexOutContext Lerp(const VertexOutContext& from, const VertexOutContext& to, float a)
	{
		VertexOutContext r = VertexOutContext();
		for (int i = 0; i < 8; i++)
		{
			r.data[i] = from.data[i] * float(1.0 - a) + to.data[i] * float(a);
		}
		return r;
	}
};

