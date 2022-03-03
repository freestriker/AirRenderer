#pragma once
#include <glm/glm.hpp>
#include <vector>
class IntersectionTester
{
public:
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
	bool Check(glm::vec3* vertexes, int vertexCount, glm::mat4 matrix)
	{
		std::vector<glm::vec4> wvBoundryVertexes = std::vector<glm::vec4>(vertexCount);
		std::vector<int> inPlaneVertexes = std::vector<int>(vertexCount, 0);
		for (int i = 0; i < vertexCount; i++)
		{
			//Log::LogVector("", glm::vec4(vertexes[i], 1.0));
			wvBoundryVertexes[i] = matrix * glm::vec4(vertexes[i], 1.0);
			//Log::LogVector("", wvBoundryVertexes[i]);

		}
		int planeCount = intersectPlanes.size();
		for (int j = 0; j < planeCount; j++)
		{
			int outCount = 0;
			for (int i = 0; i < vertexCount; i++)
			{
				if (glm::dot(wvBoundryVertexes[i], intersectPlanes[j]) >= 0)
				{
					goto Out;
				}
			}
			return false;
		Out:
			continue;
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