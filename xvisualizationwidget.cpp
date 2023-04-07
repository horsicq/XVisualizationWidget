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

    g_data.nBlockSize = 3; // TODO
    g_data.nWidth = 100; // TODO
    g_data.nHeight = 200; // TODO
    g_data.dataMethod = XVisualization::DATAMETHOD_ENTROPY;
    g_data.colorBase = this->palette().background().color();

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
    quint32 nDelta = 10;
    // TODO first add Image then descriptions
    pScene->clear();

    QGraphicsTextItem *pItemRegions = new XFileDescription(QColor(Qt::yellow), "_TST_TST_");
    pItemRegions->setPos(QPointF(0, 0));

    QImage image = XVisualization::createImage(&g_data);

    QPixmap pixmap = QPixmap::fromImage(image);

    QGraphicsPixmapItem *pItemMain = new XFileImage(QColor(Qt::green));
    pItemMain->setPixmap(pixmap);
    pItemMain->setPos(QPointF(pItemRegions->boundingRect().width() + nDelta, 0));

    QGraphicsTextItem *pItemHighlights = new XFileDescription(QColor(Qt::blue), "ABC");
    pItemHighlights->setPos(QPointF(pItemRegions->boundingRect().width() + pItemMain->boundingRect().width() + 2 * nDelta, 0));

    pScene->addItem(pItemMain);
    pScene->addItem(pItemRegions);
    pScene->addItem(pItemHighlights);

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
