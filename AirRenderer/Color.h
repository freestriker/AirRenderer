#pragma once
#include<algorithm>
#include<glm/vec4.hpp>
class Color
{
public:
	float r = 0;
	float g = 0;
	float b = 0;
	float a = 0;

	Color();
	Color(glm::vec4 color);
	Color(float r, float g, float b, float a);

	const static Color red;
	const static Color green;
	const static Color blue;
	const static Color black;
	const static Color white;

	Color operator+ (const Color& c);
	Color operator- (const Color& c);
	Color operator/ (const float& s);
	Color operator* (const float& s);
	float& operator[] (const int& i);
};