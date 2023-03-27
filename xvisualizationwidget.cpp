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
#include "xvisualizationwidget.h"
#include "ui_xvisualizationwidget.h"

XVisualizationWidget::XVisualizationWidget(QWidget *pParent) :
    QWidget(pParent),
    ui(new Ui::XVisualizationWidget)
{
    ui->setupUi(this);

    g_pDevice = nullptr;
    pScene = new QGraphicsScene(this);

    ui->graphicsViewResult->setDragMode(QGraphicsView::RubberBandDrag);
    ui->graphicsViewResult->setScene(pScene); 

    ui->horizontalSliderZoom->setMaximum(500);
    ui->horizontalSliderZoom->setValue(250);
}

XVisualizationWidget::~XVisualizationWidget()
{
    delete pScene;
    delete ui;
}

void XVisualizationWidget::setData(QIODevice *pDevice, XBinary::FT fileType, bool bAuto)
{
    this->g_pDevice = pDevice;

    XFormats::setFileTypeComboBox(fileType, g_pDevice, ui->comboBoxType);

    if (bAuto) {
        reload();
    }
}

void XVisualizationWidget::reload()
{
    // TODO
    // Resolution
    // Method

    if (g_pDevice) {
        DialogVisualizationProcess dvp(XOptions::getMainWidget(this));

        dvp.setData(g_pDevice, &g_data);

        dvp.showDialogDelay();

        if (dvp.isSuccess()) {
            reloadImage();
        }
    }
}

void XVisualizationWidget::reloadImage()
{
    // TODO first add Image then descriptions
    pScene->clear();

    QGraphicsTextItem *itemRegions = new XFileDescription(QColor(Qt::yellow), "_TST_TST_");
    itemRegions->setPos(QPointF(0, 0));
    pScene->addItem(itemRegions);

    QGraphicsTextItem *item4 = new XFileDescription(QColor(Qt::blue), "ABC");
    item4->setPos(QPointF(150, 0));
    pScene->addItem(item4);

    QImage image = XVisualization::createImage(&g_data);

//    image.save("C:\\tmp_build\\temp_out.jpeg");

    QPixmap pixmap = QPixmap::fromImage(image);

    QGraphicsPixmapItem *itemMain = new XFileImage(QColor(Qt::green));
    itemMain->setPixmap(pixmap);
    itemMain->setPos(QPointF(itemRegions->boundingRect().width(), 0));
    pScene->addItem(itemMain);

//    QGraphicsPixmapItem *item2 = new XFileImage(QColor(Qt::green));
//    item2->setPixmap(pixmap);
//    item2->setPos(QPointF(20, 20));
//    pScene->addItem(item2);

    setupMatrix(100); // TODO fix
    setupMatrix(250);
}

void XVisualizationWidget::on_horizontalSliderZoom_valueChanged(int nValue)
{
    setupMatrix(nValue);
}

void XVisualizationWidget::setupMatrix(qint32 nValue)
{
    qreal scale = qPow(qreal(2), (nValue - 250) / qreal(50));

    QTransform matrix;
    matrix.scale(scale, scale);

    ui->graphicsViewResult->setTransform(matrix);
}
