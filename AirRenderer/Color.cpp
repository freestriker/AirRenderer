#include "Color.h"
#include<algorithm>

Color::Color():Color(0, 0, 0, 0)
{

}
Color::Color(float r, float g, float b, float a)
{
	this->r = std::clamp(r, float(0), float(1));
	this->g = std::clamp(g, float(0), float(1));
	this->b = std::clamp(b, float(0), float(1));
	this->a = std::clamp(a, float(0), float(1));
}

const Color Color::red = Color(1, 0, 0, 1);
const Color Color::green = Color(0, 1, 0, 1);
const Color Color::blue = Color(0, 0, 1, 1);
const Color Color::black = Color(0, 0, 0, 1);
const Color Color::white = Color(1, 1, 1, 1);

Color Color::operator+ (const Color& c)
{
	return Color(c.r + this->r, c.g + this->g, c.b + this->b, this->a);
}
Color Color::operator- (const Color& c)
{
	return Color(-c.r + this->r, -c.g + this->g, -c.b + this->b, this->a);
}
Color Color::operator/ (const float& s)
{
	return Color(this->r / s, this->g / s, this->b / s, this->a);
}
Color Color::operator* (const float& s)
{
	return Color(this->r * s, this->g * s, this->b * s, this->a);
}
Color Color::operator* (const Color& c)
{
	return Color(this->r * c.r, this->g * c.g, this->b * c.b, this->a * c.a);
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
