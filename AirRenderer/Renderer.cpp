#include "Renderer.h"

void Renderer::Render()
{
	Drawer::DrawLine(ivec2(100, 100), ivec2(200, 200), qRgb(255, 0, 0));
	Drawer::DrawLine(ivec2(100, 100), ivec2(100, 200), qRgb(0, 255, 0));
	Drawer::DrawLine(ivec2(100, 100), ivec2(200, 100), qRgb(0, 0, 255));
	Drawer::DrawTriangle(ivec2(0, 0), ivec2(100, 100), ivec2(0, 100), qRgb(0, 0, 0));
	Drawer::DrawTriangle(ivec2(0, 0), ivec2(100, 0), ivec2(100, 100), qRgb(255, 0, 0));
	Drawer::DrawTriangle(ivec2(800, 450), ivec2(450, 230), ivec2(330, 150), qRgb(0, 255, 0));
}
void Renderer::Display()
{
	configuration.label->setPixmap(QPixmap::fromImage(configuration.canvas));
}