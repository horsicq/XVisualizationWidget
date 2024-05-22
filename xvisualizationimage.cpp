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
}

void XVisualizationImage::setData(XVisualization::DATA *pData)
{
    g_data = *pData;

    QImage image = XVisualization::createImage(&g_data);
    g_pixmap = QPixmap::fromImage(image);

    update();
}

QPixmap XVisualizationImage::getPixmap()
{
    return g_pixmap;
}

void XVisualizationImage::paintEvent(QPaintEvent *pEvent)
{
    QPainter *pPainter = new QPainter(this);

    pPainter->drawPixmap(pEvent->rect(), g_pixmap);

    // TODO

    delete pPainter;
}

void XVisualizationImage::mousePressEvent(QMouseEvent *pEvent)
{
    qint32 nWidth= width();
    qint32 nHeight = height();
    qint32 nX = pEvent->x();
    qint32 nY = pEvent->y();

    qint32 _nY = (nY * g_data.nHeight) / nHeight;
    qint32 _nX = (nX * g_data.nWidth) / nWidth;

    qDebug("X: %d", _nY);
    qDebug("Y: %d", _nX);
}
