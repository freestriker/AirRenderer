#include "Renderer.h"

void Renderer::Render()
{
	Drawer::DrawLine(ivec2(100, 100), ivec2(200, 200), Color(1, 1, 0, 1), *buffer);
	Drawer::DrawLine(ivec2(100, 100), ivec2(100, 200), Color(0, 1, 0, 1), *buffer);
	Drawer::DrawLine(ivec2(100, 100), ivec2(200, 100), Color(0, 0, 1, 1), *buffer);
	Drawer::DrawTriangle(ivec2(0, 0), ivec2(100, 100), ivec2(0, 100), Color(1, 0, 0, 1), *buffer);
	Drawer::DrawTriangle(ivec2(0, 0), ivec2(100, 0), ivec2(100, 100), Color(1, 0, 1, 1), *buffer);
	Drawer::DrawTriangle(ivec2(799, 449), ivec2(449, 230), ivec2(330, 150), Color(0, 1, 0, 1), *buffer);
	
	for (int x = 0; x < configuration.resolution.width; x++)
	{
		for (int y = 0; y < configuration.resolution.height; y++)
		{
			Color color = buffer->GetData(x, y);
			configuration.canvas.setPixelColor(x, y, QColor(color.r * color.a * 255, color.g * color.a * 255, color.b * color.a * 255));
			
		}
	}
}
void Renderer::Display()
{
	configuration.label->setPixmap(QPixmap::fromImage(configuration.canvas));
}

Renderer::Renderer()
{
	buffer = new Buffer<Color>(configuration.resolution.width, configuration.resolution.height);
}