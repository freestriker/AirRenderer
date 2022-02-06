#include "Configuration.h"
Configuration configuration = Configuration();

glm::mat4 Configuration::GetScreenMatrix()
{
    return glm::mat4(
        float(resolution.width) / 2.0, 0, 0, 0,
        0, float(-resolution.height) / 2.0, 0, 0,
        0, 0, 1, 0,
		float(resolution.width) / 2.0, float(resolution.height) / 2.0, 1, 1
    );
}
void Configuration::Display()
{
	for (int x = 0; x < configuration.resolution.width; x++)
	{
		for (int y = 0; y < configuration.resolution.height; y++)
		{
			Color color = colorBuffer->GetData(x, y);
			configuration.canvas.setPixelColor(x, y, QColor(color.r * color.a * 255, color.g * color.a * 255, color.b * color.a * 255));

		}
	}
	configuration.label->setPixmap(QPixmap::fromImage(configuration.canvas));

}

Configuration::Configuration()
{
	this->colorBuffer = new Buffer<Color>(800, 450);
}
