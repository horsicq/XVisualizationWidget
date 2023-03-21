#ifndef XVISUALIZATIONWIDGET_H
#define XVISUALIZATIONWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include "xformats.h"

namespace Ui {
class XVisualizationWidget;
}

class XVisualizationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit XVisualizationWidget(QWidget *pParent = nullptr);
    ~XVisualizationWidget();

    static QImage createImage();

    void setData(QIODevice *pDevice, XBinary::FT fileType, bool bAuto = false);
    void reload();

private:
    Ui::XVisualizationWidget *ui;
    QIODevice *g_pDevice;
};

#endif // XVISUALIZATIONWIDGET_H
