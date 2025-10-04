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
#include "xfileimage.h"

XFileImage::XFileImage(const QColor &color)
{
    g_color = color;
    m_pData = nullptr;
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}

QRectF XFileImage::boundingRect() const
{
    QRectF rect(0, 0, 300, 600);

    if (m_pData) {
        rect.setWidth(m_pData->nWidth * m_pData->nBlockSize);
        rect.setHeight(m_pData->nHeight * m_pData->nBlockSize);
    }

    return rect;
}

QPainterPath XFileImage::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void XFileImage::setCustomData(XVisualization::DATA *pData, QPixmap &pixmap)
{
    m_pData = pData;

    setPixmap(pixmap);
}

// void XFileImage::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
//{
//     Q_UNUSED(pWidget);

//    const qreal rZoom = pOption->levelOfDetailFromTransform(pPainter->worldTransform());

//    qDebug("Zoom: %f", rZoom);

//    QRectF _rectOrig = boundingRect();

//    QRect rect = QRectF(_rectOrig.left(), _rectOrig.top(), _rectOrig.width() * rZoom, _rectOrig.height() * rZoom).toRect();
//    pPainter->drawPixmap(rect, pixmap());
//}

void XFileImage::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void XFileImage::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void XFileImage::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}
