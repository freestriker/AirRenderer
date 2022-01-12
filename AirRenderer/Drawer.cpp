#include "Drawer.h"


void Drawer::DrawLine(ivec2 startPosition, ivec2 endPosition, QRgb color)
{
	bool isSteep = false;
	ivec2 difference = endPosition - startPosition;
	if (abs(difference.y) > abs(difference.x))
	{
		isSteep = true;

		int t = startPosition.x;
		startPosition.x = startPosition.y;
		startPosition.y = t;

		t = endPosition.x;
		endPosition.x = endPosition.y;
		endPosition.y = t;
	}

	if (endPosition.x < startPosition.x)
	{
		ivec2 t = startPosition;
		startPosition = endPosition;
		endPosition = t;
	}

	difference = endPosition - startPosition;
	int deltaAbs2X = abs(difference.x) * 2;
	int deltaAbs2Y = abs(difference.y) * 2;
	int d = 0;
	int dy = difference.y > 0 ? 1 : -1;
	if (isSteep)
	{
		for (int x = startPosition.x, y = startPosition.y; x <= endPosition.x; x++)
		{
			configuration.canvas.setPixel(y, x, color);

			d += deltaAbs2Y;
			if (d > difference.x)
			{
				y += dy;
				d -= deltaAbs2X;
			}
		}
	}
	else
	{
		for (int x = startPosition.x, y = startPosition.y; x <= endPosition.x; x++)
		{
			configuration.canvas.setPixel(x, y, color);

			d += deltaAbs2Y;
			if (d > difference.x)
			{
				y += dy;
				d -= deltaAbs2X;
			}
		}
	}
}