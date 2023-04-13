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

XVisualizationWidget::XVisualizationWidget(QWidget *pParent) : QWidget(pParent), ui(new Ui::XVisualizationWidget)
{
    ui->setupUi(this);

    g_pDevice = nullptr;
    pScene = new QGraphicsScene(this);

    ui->graphicsViewResult->setDragMode(QGraphicsView::RubberBandDrag);
    ui->graphicsViewResult->setScene(pScene);

    ui->horizontalSliderZoom->setMaximum(500);
    ui->horizontalSliderZoom->setValue(250);

    ui->splitterVisualization->setStretchFactor(0, 1);
    ui->splitterVisualization->setStretchFactor(1, 0);
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
    // TODO
    // Get XBinary:: regions, highlights, resolution
    // TODO insert to QListWidgets

    g_data.nBlockSize = 3;  // TODO
    g_data.nWidth = 100;    // TODO
    g_data.nHeight = 200;   // TODO
    g_data.dataMethod = XVisualization::DATAMETHOD_ENTROPY;
    g_data.colorBase = this->palette().background().color();
    g_data.fileFormat = (XBinary::FT)(ui->comboBoxType->currentData().toInt());

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
    qreal rDelta = 5;
    // TODO first add Image then descriptions
    pScene->clear();

    qreal rRegionsSize = 0;
    qreal rHighlightsSize = 0;

    QList<QGraphicsTextItem *> listRegions;
    QList<QGraphicsTextItem *> listHighlights;

    {
        qint32 nNumberOfRecords = g_data.listRegions.count();
        qreal rPosition = 0;

        for (qint32 i = 0; i < nNumberOfRecords; i++) {
            QGraphicsTextItem *pItemRegion = new XFileDescription(g_data.listRegions.at(i).color, g_data.listRegions.at(i).sName);
            pItemRegion->setPos(QPointF(0, rPosition));

            rRegionsSize = qMax(pItemRegion->boundingRect().width(), rRegionsSize);

            rPosition += pItemRegion->boundingRect().height();
            rPosition += rDelta;

            listRegions.append(pItemRegion);
        }
    }

    QImage image = XVisualization::createImage(&g_data);

    QPixmap pixmap = QPixmap::fromImage(image);

    QGraphicsPixmapItem *pItemMain = new XFileImage(QColor(Qt::green));
    pItemMain->setPixmap(pixmap);
    pItemMain->setPos(QPointF(rRegionsSize + rDelta, 0));

    {
        qint32 nNumberOfRecords = g_data.listHighlights.count();
        qreal rPosition = 0;

        for (qint32 i = 0; i < nNumberOfRecords; i++) {
            QGraphicsTextItem *pItemHighlight = new XFileDescription(g_data.listHighlights.at(i).color, g_data.listHighlights.at(i).sName);
            pItemHighlight->setPos(QPointF(rRegionsSize + pItemMain->boundingRect().width() + 2 * rDelta, 0));

            rHighlightsSize = qMax(pItemHighlight->boundingRect().width(), rHighlightsSize);

            rPosition += pItemHighlight->boundingRect().height();
            rPosition += rDelta;

            listHighlights.append(pItemHighlight);
        }
    }

    pScene->addItem(pItemMain);

    {
        qint32 nNumberOfRecords = listRegions.count();

        for (qint32 i = 0; i < nNumberOfRecords; i++) {
            pScene->addItem(listRegions.at(i));
        }
    }

    {
        qint32 nNumberOfRecords = listHighlights.count();

        for (qint32 i = 0; i < nNumberOfRecords; i++) {
            pScene->addItem(listHighlights.at(i));
        }
    }

    setupMatrix(100);  // TODO fix
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

void XVisualizationWidget::on_pushButtonVisualizationSave_clicked()
{
    QString sFilter = XOptions::getImageFilter();

    QString sFileName = XBinary::getResultFileName(g_pDevice, QString("%1.png").arg(tr("Visualization")));

    sFileName = QFileDialog::getSaveFileName(this, tr("Save"), sFileName, sFilter);

    if (!sFileName.isEmpty()) {
        QPixmap pixMap = ui->graphicsViewResult->viewport()->grab();
        pixMap.save(sFileName);
    }
}

void XVisualizationWidget::on_listWidgetRegions_itemSelectionChanged()
{
    reloadImage();
}

void XVisualizationWidget::on_listWidgetHighlights_itemSelectionChanged()
{
    reloadImage();
}
