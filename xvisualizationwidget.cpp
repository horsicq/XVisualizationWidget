#include "xvisualizationwidget.h"
#include "ui_xvisualizationwidget.h"

XVisualizationWidget::XVisualizationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::XVisualizationWidget)
{
    ui->setupUi(this);
}

XVisualizationWidget::~XVisualizationWidget()
{
    delete ui;
}
