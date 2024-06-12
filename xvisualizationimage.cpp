/* Copyright (c) 2023-2024 hors<horsicq@gmail.com>
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

XVisualizationImage::XVisualizationImage(QWidget *pParent) : QWidget(pParent)
{
    _clear();
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

void XVisualizationImage::adjust()
{
    qint32 nIndex = ((g_data.nWidth * g_nY) + g_nX);

    qint64 nOffset = 0;

    if (nIndex < g_data.nWidth * g_data.nHeight) {
        nOffset = g_data.listParts.at(nIndex).nOffset;
    }

    // qDebug("Offset: %x = %d", nOffset, (qint32)nOffset);
}

void XVisualizationImage::_clear()
{
    g_nY = 0;
    g_nX = 0;
    g_nCurrentIndex = 0;
    g_data = {};
    g_pixmap = QPixmap();
}

void XVisualizationImage::paintEvent(QPaintEvent *pEvent)
{
    QPainter *pPainter = new QPainter(this);

    pPainter->drawPixmap(pEvent->rect(), g_pixmap);

    if (g_data.nWidth > 0) {
        qint32 nWidth = width();
        qint32 nHeight = height();

        double dRectWidth = (double)nWidth / (double)g_data.nWidth;
        double dRectHeight = (double)nHeight / (double)g_data.nHeight;
        double dX = (double)(g_nX * nWidth) / (double)g_data.nWidth;
        double dY = (double)(g_nY * nHeight) / (double)g_data.nHeight;

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
    qint32 nX = pEvent->x();
    qint32 nY = pEvent->y();

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
