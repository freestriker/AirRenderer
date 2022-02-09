#include "Color.h"

Color::Color():Color(0, 0, 0, 0)
{

}
Color::Color(glm::vec4 color): Color(color.x, color.y, color.z, color.w)
{
}
Color::Color(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

const Color Color::red = Color(1, 0, 0, 1);
const Color Color::green = Color(0, 1, 0, 1);
const Color Color::blue = Color(0, 0, 1, 1);
const Color Color::black = Color(0, 0, 0, 1);
const Color Color::white = Color(1, 1, 1, 1);

Color Color::operator+ (const Color& c)
{
	return Color(std::clamp(c.r + this->r, float(0), float(1)), 
		std::clamp(c.g + this->g, float(0), float(1)), 
		std::clamp(c.b + this->b, float(0), float(1)), 
		std::clamp(c.a + this->a, float(0), float(1)));
}
Color Color::operator- (const Color& c)
{
	return Color(std::clamp(-c.r + this->r, float(0), float(1)),
		std::clamp(-c.g + this->g, float(0), float(1)),
		std::clamp(-c.b + this->b, float(0), float(1)),
		std::clamp(-c.a + this->a, float(0), float(1)));
}
Color Color::operator/ (const float& s)
{
	return Color(std::clamp(this->r / s, float(0), float(1)),
		std::clamp(this->g / s, float(0), float(1)),
		std::clamp(this->b / s, float(0), float(1)),
		std::clamp(this->a / s, float(0), float(1)));
}
Color Color::operator* (const float& s)
{
	return Color(std::clamp(this->r * s, float(0), float(1)),
		std::clamp(this->g * s, float(0), float(1)),
		std::clamp(this->b * s, float(0), float(1)),
		std::clamp(this->a * s, float(0), float(1)));
}
float& Color::operator[] (const int& i)
{
	switch (i)
	{
	case 0:
		return r;
	case 1:
		return g;
	case 2:
		return b;
	case 3:
		return a;
	default:
		return r;
	}
}
