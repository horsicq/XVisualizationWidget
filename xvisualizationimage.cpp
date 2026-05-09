/* Copyright (c) 2023-2026 hors<horsicq@gmail.com>
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
#include "xvisualizationimage.h"

XVisualizationImage::XVisualizationImage(QWidget *pParent) : XShortcutsWidget(pParent)
{
    _clear();

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenu(QPoint)));
}

void XVisualizationImage::setData(XVisualization::DATA *pData)
{
    g_data = *pData;

    QImage image = XVisualization::createImage(&g_data);
    g_pixmap = QPixmap::fromImage(image);

    adjust();
    update();
}

void XVisualizationImage::clear()
{
    _clear();
    update();
}

QPixmap XVisualizationImage::getPixmap()
{
    return g_pixmap;
}

void XVisualizationImage::adjustView()
{
    // TODO
}

void XVisualizationImage::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)
    adjust();
    update();
}

void XVisualizationImage::adjust()
{
}

void XVisualizationImage::_clear()
{
    g_nY = 0;
    g_nX = 0;
    g_nCurrentIndex = 0;
    g_data = {};
    g_pixmap = QPixmap();
}

void XVisualizationImage::customContextMenu(const QPoint &pos)
{
    Q_UNUSED(pos)
}

void XVisualizationImage::paintEvent(QPaintEvent *pEvent)
{
    QPainter *pPainter = new QPainter(this);

    pPainter->drawPixmap(pEvent->rect(), g_pixmap);

    if (g_data.nWidth > 0) {
        qint32 nWidth = width();
        qint32 nHeight = height();

        double dRectWidth = static_cast<double>(nWidth) / static_cast<double>(g_data.nWidth);
        double dRectHeight = static_cast<double>(nHeight) / static_cast<double>(g_data.nHeight);
        double dX = static_cast<double>(g_nX * nWidth) / static_cast<double>(g_data.nWidth);
        double dY = static_cast<double>(g_nY * nHeight) / static_cast<double>(g_data.nHeight);

        QRectF rect;

        rect.setLeft(dX);
        rect.setTop(dY);
        rect.setWidth(dRectWidth);
        rect.setHeight(dRectHeight);

        pPainter->fillRect(rect, QBrush(Qt::red));
    }

    delete pPainter;
}

void XVisualizationImage::mousePressEvent(QMouseEvent *pEvent)
{
    qint32 nX = pEvent->pos().x();
    qint32 nY = pEvent->pos().y();

    g_nY = (nY * g_data.nHeight) / height();
    g_nX = (nX * g_data.nWidth) / width();

    g_nCurrentIndex = (g_nY * g_data.nWidth) + g_nX;

    if (g_nCurrentIndex < g_data.listParts.count()) {
        emit currentLocationChanged(g_data.listParts.at(g_nCurrentIndex).nOffset, XBinary::LT_OFFSET, g_data.nFileBlockSize);
    }

    adjust();
    update();
}

void XVisualizationImage::resizeEvent(QResizeEvent *pEvent)
{
    adjust();
    QWidget::resizeEvent(pEvent);
}

void XVisualizationImage::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
