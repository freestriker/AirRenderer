#pragma once
#include <QImage>
#include <QLabel>

typedef struct DisplayResolution
{
public:
	int width = 800;
	int height = 450;
} Resolution;

class Configuration
{
public:
	Resolution resolution;
	QImage canvas;
	QLabel* label;
};

extern Configuration configuration;
