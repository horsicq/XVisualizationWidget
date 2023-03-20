#ifndef XVISUALIZATIONWIDGET_H
#define XVISUALIZATIONWIDGET_H

#include <QWidget>

namespace Ui {
class XVisualizationWidget;
}

class XVisualizationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit XVisualizationWidget(QWidget *parent = nullptr);
    ~XVisualizationWidget();

private:
    Ui::XVisualizationWidget *ui;
};

#endif // XVISUALIZATIONWIDGET_H
