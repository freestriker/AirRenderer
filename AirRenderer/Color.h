#pragma once
class Color
{
public:
	float r = 0;
	float g = 0;
	float b = 0;
	float a = 0;

	Color();
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
	Color operator*(const Color& c);
	float& operator[] (const int& i);
};