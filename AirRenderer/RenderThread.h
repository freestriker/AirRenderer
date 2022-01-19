#pragma once
#include <QtWidgets>
#include "Renderer.h"

class RenderThread : public QThread
{
	Q_OBJECT
public:
	QTimer* timer;
	void Run();
	RenderThread(QObject* parent);
	Renderer* renderer;
private slots:void Render();
};