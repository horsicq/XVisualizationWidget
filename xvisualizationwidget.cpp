/* Copyright (c) 2023-2025 hors<horsicq@gmail.com>
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

XVisualizationWidget::XVisualizationWidget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::XVisualizationWidget)
{
    ui->setupUi(this);

    XOptions::adjustToolButton(ui->toolButtonVisualizationReload, XOptions::ICONTYPE_RELOAD);
    XOptions::adjustToolButton(ui->toolButtonVisualizationSave, XOptions::ICONTYPE_SAVE);

    ui->comboBoxType->setToolTip(tr("Type"));
    ui->comboBoxMapMode->setToolTip(tr("Mode"));
    ui->comboBoxMethods->setToolTip(tr("Methods"));
    ui->comboBoxMethod->setToolTip(tr("Method"));
    ui->spinBoxBlockSize->setToolTip(tr("Block size"));
    ui->spinBoxWidth->setToolTip(tr("Width"));
    ui->spinBoxHeight->setToolTip(tr("Height"));
    ui->horizontalSliderZoom->setToolTip(tr("Zoom"));
    ui->listWidgetRegions->setToolTip(tr("Regions"));
    ui->listWidgetHighlights->setToolTip(tr("Highlights"));
    ui->tabWidgetVisualization->setToolTip(tr("Visualization"));
    ui->toolButtonVisualizationReload->setToolTip(tr("Reload"));
    ui->toolButtonVisualizationSave->setToolTip(tr("Save"));

    connect(ui->widgetImage, SIGNAL(currentLocationChanged(quint64, qint32, qint64)), this, SIGNAL(currentLocationChanged(quint64, qint32, qint64)));

    m_pDevice = nullptr;
    g_pScene = new QGraphicsScene(this);

    ui->graphicsViewResult->setDragMode(QGraphicsView::RubberBandDrag);
    ui->graphicsViewResult->setScene(g_pScene);

    ui->horizontalSliderZoom->setMaximum(500);
    ui->horizontalSliderZoom->setValue(250);

    ui->splitterVisualization->setStretchFactor(0, 1);
    ui->splitterVisualization->setStretchFactor(1, 0);

    ui->comboBoxMethods->setData(XVisualization::getMethodFlags(), XComboBoxEx::CBTYPE_FLAGS, 0, tr("Methods"));
    ui->comboBoxMethods->setValue(XVisualization::getDefaultMethodFlag());
    ui->comboBoxMethods->setItemEnabled(1, false);

    ui->spinBoxBlockSize->setValue(3);
}

XVisualizationWidget::~XVisualizationWidget()
{
    delete g_pScene;
    delete ui;
}

void XVisualizationWidget::setData(QIODevice *pDevice, XBinary::FT fileType, bool bAuto)
{
    this->m_pDevice = pDevice;

    qint32 nWidth = 1;
    qint32 nHeight = 1;

    if (m_pDevice) {
        XBinary::FT _fileType = XFormats::setFileTypeComboBox(fileType, m_pDevice, ui->comboBoxType);
        XFormats::getMapModesList(_fileType, ui->comboBoxMapMode);

        qint32 nPieces = (m_pDevice->size()) / 0x300;

        if (nPieces > 3) {
            nWidth = qMin(nPieces, 100);
            nHeight = qMin(nPieces * 2, 200);
        }
    }

    ui->spinBoxWidth->setValue(nWidth);
    ui->spinBoxHeight->setValue(nHeight);

    if (bAuto) {
        reload();
    }
}

void XVisualizationWidget::reload()
{
    const bool bBlocked1 = ui->listWidgetRegions->blockSignals(true);
    const bool bBlocked2 = ui->listWidgetHighlights->blockSignals(true);
    const bool bBlocked3 = ui->comboBoxMethod->blockSignals(true);

    // TODO
    // Get XBinary:: regions, highlights, resolution
    bool bReloadImage = false;

    if (m_pDevice) {
        g_data.nWidth = ui->spinBoxWidth->value();
        g_data.nHeight = ui->spinBoxHeight->value();
        g_data.fileFormat = (XBinary::FT)(ui->comboBoxType->currentData().toInt());
        g_data.mapMode = (XBinary::MAPMODE)(ui->comboBoxMapMode->currentData().toInt());
        g_data.nMethodsFlags = ui->comboBoxMethods->getValue().toULongLong();

        if (m_pDevice) {
            XVisualization visualisation;
            XDialogProcess dvp(XOptions::getMainWidget(this), &visualisation);
            dvp.setGlobal(getShortcuts(), getGlobalOptions());
            visualisation.setData(m_pDevice, &g_data, dvp.getPdStruct());
            dvp.start();
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
    } else {
        bReloadImage = true;
    }

    ui->comboBoxMethod->clear();

    QList<XVisualization::DATAMETHOD> listMethods = XVisualization::getMethodsListFromFlags(g_data.nMethodsFlags);

    qint32 nNumberOfMethods = listMethods.count();

    for (qint32 i = 0; i < nNumberOfMethods; i++) {
        ui->comboBoxMethod->addItem(XVisualization::methodToString(listMethods.at(i)), listMethods.at(i));
    }

    if (nNumberOfMethods > 1) {
        ui->comboBoxMethod->setCurrentIndex(1);
    }

    if (bReloadImage) {
        reloadImage();
    }

    ui->listWidgetRegions->blockSignals(bBlocked1);
    ui->listWidgetHighlights->blockSignals(bBlocked2);
    ui->comboBoxMethod->blockSignals(bBlocked3);
}

void XVisualizationWidget::reloadImage()
{
    g_pScene->clear();

    if (m_pDevice) {
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
        // g_data.colorBase = this->palette().background().color();
        g_data.colorBase = this->palette().window().color();
        g_data.dataMethod = (XVisualization::DATAMETHOD)(ui->comboBoxMethod->currentData().toInt());

        qreal rDelta = 5;
        // TODO first add Image then descriptions

        qreal rRegionsSize = 150;
        qreal rHighlightsSize = 150;

        QList<QGraphicsTextItem *> listRegions;
        QList<QGraphicsTextItem *> listHighlights;

        {
            qint32 nNumberOfRecords = g_data.listRegions.count();
            qreal rPosition = 0;

            for (qint32 i = 0; i < nNumberOfRecords; i++) {
                if (g_data.listRegions.at(i).bIsEnabled) {
                    QGraphicsTextItem *pItemRegion = new XFileDescription(g_pScene, g_data.listRegions.at(i).color, g_data.listRegions.at(i).sName);
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

        XFileImage *pItemMain = new XFileImage(QColor(Qt::green));
        pItemMain->setCustomData(&g_data, pixmap);
        pItemMain->setPos(QPointF(rRegionsSize + rDelta, 0));

        {
            qint32 nNumberOfRecords = g_data.listHighlights.count();
            qreal rPosition = 0;

            for (qint32 i = 0; i < nNumberOfRecords; i++) {
                if (g_data.listHighlights.at(i).bIsEnabled) {
                    QGraphicsTextItem *pItemHighlight = new XFileDescription(g_pScene, g_data.listHighlights.at(i).color, g_data.listHighlights.at(i).sName);
                    pItemHighlight->setPos(QPointF(rRegionsSize + pItemMain->boundingRect().width() + 2 * rDelta, rPosition));

                    rHighlightsSize = qMax(pItemHighlight->boundingRect().width(), rHighlightsSize);

                    rPosition += pItemHighlight->boundingRect().height();
                    rPosition += rDelta;

                    listHighlights.append(pItemHighlight);
                }
            }
        }

        g_pScene->addItem(pItemMain);

        {
            qint32 nNumberOfRecords = listRegions.count();

            for (qint32 i = 0; i < nNumberOfRecords; i++) {
                g_pScene->addItem(listRegions.at(i));
            }
        }

        {
            qint32 nNumberOfRecords = listHighlights.count();

            for (qint32 i = 0; i < nNumberOfRecords; i++) {
                g_pScene->addItem(listHighlights.at(i));
            }
        }

        ui->horizontalSliderZoom->setValue(250);
        setupMatrix(100);  // TODO fix
        setupMatrix(250);

        ui->widgetImage->setData(&g_data);
    } else {
        ui->widgetImage->clear();
    }
}

void XVisualizationWidget::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    ui->widgetImage->setGlobal(pShortcuts, pXOptions);
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
}

void XVisualizationWidget::adjustView()
{
    getGlobalOptions()->adjustWidget(this, XOptions::ID_VIEW_FONT_CONTROLS);
    getGlobalOptions()->adjustWidget(ui->graphicsViewResult, XOptions::ID_VIEW_FONT_CONTROLS);

    {
        QString sFont = getGlobalOptions()->getValue(XOptions::ID_VIEW_FONT_CONTROLS).toString();

        QFont _font;
        if ((sFont != "") && _font.fromString(sFont)) {
            g_pScene->setFont(_font);
        }
    }
}

void XVisualizationWidget::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)
    reload();
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

void XVisualizationWidget::on_toolButtonVisualizationSave_clicked()
{
    QString sFilter = XOptions::getImageFilter();
    QString sFileName = XBinary::getResultFileName(m_pDevice, QString("%1.png").arg(tr("Visualization")));

    sFileName = QFileDialog::getSaveFileName(this, tr("Save"), sFileName, sFilter);

    if (!sFileName.isEmpty()) {
        if (ui->tabWidgetVisualization->currentIndex() == 0) {
            QPixmap pixMap = ui->widgetImage->getPixmap();
            pixMap.save(sFileName);
        } else if (ui->tabWidgetVisualization->currentIndex() == 1) {
            QPixmap pixMap = ui->graphicsViewResult->viewport()->grab();
            pixMap.save(sFileName);
        }
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

void XVisualizationWidget::on_toolButtonVisualizationReload_clicked()
{
    reload();
}

void XVisualizationWidget::on_spinBoxBlockSize_valueChanged(int nValue)
{
    Q_UNUSED(nValue)
    reload();
}

void XVisualizationWidget::on_comboBoxType_currentIndexChanged(int nIndex)
{
    Q_UNUSED(nIndex)

    XBinary::FT fileType = (XBinary::FT)(ui->comboBoxType->currentData().toInt());
    XFormats::getMapModesList(fileType, ui->comboBoxMapMode);

    reload();
}

void XVisualizationWidget::on_comboBoxMapMode_currentIndexChanged(int nIndex)
{
    Q_UNUSED(nIndex)
    reload();
}

void XVisualizationWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
