#pragma once
#include <glm/glm.hpp>
#include <vector>
class IntersectionTester
{
	std::vector<glm::vec4> intersectPlanes;
	bool Check(glm::vec3* vertexes, int vertexCount)
	{
		for (int i = 0; i < vertexCount; i++)
		{
			glm::vec4 v = glm::vec4(vertexes[i], 1.0);
			int outCount = 0;
			for each (glm::vec4 plane in intersectPlanes)
			{
				if (glm::dot(v, plane) < 0)
				{
					outCount++;
				}

			}
			if (outCount == intersectPlanes.size())
			{
				return false;
			}
		}
		return true;
	}
	IntersectionTester(glm::vec4* planes, int planeCount)
	{
		intersectPlanes = std::vector<glm::vec4>(planeCount);
		for (int i = 0; i < planeCount; i++)
		{
			intersectPlanes[i] = planes[i];
		}
	}
};