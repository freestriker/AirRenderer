#pragma once
#include <QImage>
#include <QLabel>

typedef struct DisplayResolution
{
public:
	int width = 1600;
	int height = 900;
} Resolution;

class Configuration
{
public:
	Resolution resolution;
	QImage canvas;
	QLabel* label;
};

extern Configuration configuration;
