#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AirRenderer.h"

class AirRenderer : public QMainWindow
{
    Q_OBJECT

public:
    AirRenderer(QWidget *parent = Q_NULLPTR);

private:
    Ui::AirRendererClass ui;
};
