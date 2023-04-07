/* Copyright (c) 2023 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef XVISUALIZATIONWIDGET_H
#define XVISUALIZATIONWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include "xformats.h"
#include "xfileimage.h"
#include "xfiledescription.h"
#include "dialogvisualizationprocess.h"

namespace Ui {
class XVisualizationWidget;
}

class XVisualizationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit XVisualizationWidget(QWidget *pParent = nullptr);
    ~XVisualizationWidget();

    void setData(QIODevice *pDevice, XBinary::FT fileType, bool bAuto = false);
    void reload();
    void reloadImage();

private slots:
    void on_horizontalSliderZoom_valueChanged(int nValue);
    void setupMatrix(qint32 nValue);
    void on_pushButtonVisualizationSave_clicked();
    void on_listWidgetRegions_itemSelectionChanged();
    void on_listWidgetHighlights_itemSelectionChanged();

private:
    Ui::XVisualizationWidget *ui;
    QIODevice *g_pDevice;
    QGraphicsScene *pScene;
    XVisualization::DATA g_data;
};

#endif // XVISUALIZATIONWIDGET_H
