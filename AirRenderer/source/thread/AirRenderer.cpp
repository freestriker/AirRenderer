#include "include/thread/AirRenderer.h"
#include "stdafx.h"
#include "include/core_object/Configuration.h"
#include "include/thread/RenderThread.h"

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