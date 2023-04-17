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

    ui->comboBoxMethod->addItem("", XVisualization::DATAMETHOD_NONE);
    ui->comboBoxMethod->addItem(tr("Entropy"), XVisualization::DATAMETHOD_ENTROPY);

    ui->comboBoxMethod->setCurrentIndex(1); // Set Entropy

    ui->spinBoxWidth->setValue(100);
    ui->spinBoxHeight->setValue(200);
    ui->spinBoxBlockSize->setValue(3);
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
    const bool bBlocked1 = ui->listWidgetRegions->blockSignals(true);
    const bool bBlocked2 = ui->listWidgetHighlights->blockSignals(true);

    // TODO
    // Get XBinary:: regions, highlights, resolution

    if (g_pDevice) {
        bool bReloadImage = false;

        g_data.nWidth = ui->spinBoxWidth->value();
        g_data.nHeight = ui->spinBoxHeight->value();
        g_data.fileFormat = (XBinary::FT)(ui->comboBoxType->currentData().toInt());

        if (g_pDevice) {
            DialogVisualizationProcess dvp(XOptions::getMainWidget(this));

            dvp.setData(g_pDevice, &g_data);

            dvp.showDialogDelay();

            bReloadImage = dvp.isSuccess();
        }

        {
            ui->listWidgetRegions->clear();

            qint32 nNumberOfRecords = g_data.listRegions.count();

            for (qint32 i = 0; i < nNumberOfRecords; i++) {
                QListWidgetItem *pItem = new QListWidgetItem;
                pItem->setText(g_data.listRegions.at(i).sName);
                pItem->setData(Qt::UserRole, i);
                pItem->setCheckState(Qt::Checked);

                ui->listWidgetRegions->addItem(pItem);
            }
        }

        {
            ui->listWidgetHighlights->clear();

            qint32 nNumberOfRecords = g_data.listHighlights.count();

            for (qint32 i = 0; i < nNumberOfRecords; i++) {
                QListWidgetItem *pItem = new QListWidgetItem;
                pItem->setText(g_data.listHighlights.at(i).sName);
                pItem->setData(Qt::UserRole, i);
                pItem->setCheckState(Qt::Checked);

                ui->listWidgetHighlights->addItem(pItem);
            }
        }

        if (bReloadImage) {
            reloadImage();
        }
    }

    ui->listWidgetRegions->blockSignals(bBlocked1);
    ui->listWidgetHighlights->blockSignals(bBlocked2);
}

void XVisualizationWidget::reloadImage()
{
    if (g_pDevice) {
        {
            qint32 nNumberOfRecords = ui->listWidgetRegions->count();

            for (qint32 i = 0; i < nNumberOfRecords; i++) {
                bool bIsEnabled = (ui->listWidgetRegions->item(i)->checkState() == Qt::Checked);

                g_data.listRegions[i].bIsEnabled = bIsEnabled;
            }
        }
        {
            qint32 nNumberOfRecords = ui->listWidgetHighlights->count();

            for (qint32 i = 0; i < nNumberOfRecords; i++) {
                bool bIsEnabled = (ui->listWidgetHighlights->item(i)->checkState() == Qt::Checked);

                g_data.listHighlights[i].bIsEnabled = bIsEnabled;
            }
        }

        g_data.nBlockSize = ui->spinBoxBlockSize->value();
        g_data.colorBase = this->palette().background().color();
        g_data.dataMethod = (XVisualization::DATAMETHOD)(ui->comboBoxMethod->currentData().toInt());

        qreal rDelta = 5;
        // TODO first add Image then descriptions
        pScene->clear();

        qreal rRegionsSize = 150;
        qreal rHighlightsSize = 150;

        QList<QGraphicsTextItem *> listRegions;
        QList<QGraphicsTextItem *> listHighlights;

        {
            qint32 nNumberOfRecords = g_data.listRegions.count();
            qreal rPosition = 0;

            for (qint32 i = 0; i < nNumberOfRecords; i++) {
                if (g_data.listRegions.at(i).bIsEnabled) {
                    QGraphicsTextItem *pItemRegion = new XFileDescription(g_data.listRegions.at(i).color, g_data.listRegions.at(i).sName);
                    pItemRegion->setPos(QPointF(0, rPosition));

                    rRegionsSize = qMax(pItemRegion->boundingRect().width(), rRegionsSize);

                    rPosition += pItemRegion->boundingRect().height();
                    rPosition += rDelta;

                    listRegions.append(pItemRegion);
                }
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
                if (g_data.listHighlights.at(i).bIsEnabled) {
                    QGraphicsTextItem *pItemHighlight = new XFileDescription(g_data.listHighlights.at(i).color, g_data.listHighlights.at(i).sName);
                    pItemHighlight->setPos(QPointF(rRegionsSize + pItemMain->boundingRect().width() + 2 * rDelta, 0));

                    rHighlightsSize = qMax(pItemHighlight->boundingRect().width(), rHighlightsSize);

                    rPosition += pItemHighlight->boundingRect().height();
                    rPosition += rDelta;

                    listHighlights.append(pItemHighlight);
                }
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

        ui->horizontalSliderZoom->setValue(250);
        setupMatrix(100);  // TODO fix
        setupMatrix(250);
    }
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

void XVisualizationWidget::on_comboBoxMethod_currentIndexChanged(int nIndex)
{
    Q_UNUSED(nIndex)

    reloadImage();
}

void XVisualizationWidget::on_listWidgetRegions_itemChanged(QListWidgetItem *pItem)
{
    Q_UNUSED(pItem)

    reloadImage();
}

void XVisualizationWidget::on_listWidgetHighlights_itemChanged(QListWidgetItem *pItem)
{
    Q_UNUSED(pItem)

    reloadImage();
}

void XVisualizationWidget::on_pushButtonVisualizationReload_clicked()
{
    reload();
}

void XVisualizationWidget::on_spinBoxBlockSize_valueChanged(int nValue)
{
    Q_UNUSED(nValue)
    reload();
}
