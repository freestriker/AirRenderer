#pragma once
#include <QtWidgets>

class RenderThread : public QThread
{
	Q_OBJECT
public:
	QTimer* timer;
	void Run();
	RenderThread(QObject* parent);
private slots:void Render();
};