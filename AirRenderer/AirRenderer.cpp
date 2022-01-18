#include "AirRenderer.h"
#include "stdafx.h"
#include "Configuration.h"
#include "RenderThread.h"

AirRenderer::AirRenderer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    QLabel* label = this->centralWidget()->findChild<QLabel*>("label");
    QSize size = QSize(configuration.resolution.width, configuration.resolution.height);
    label->setFixedSize(size);
    this->setFixedSize(size);
    configuration.canvas = QImage(size, QImage::Format_RGB32);
    configuration.label = label;

}