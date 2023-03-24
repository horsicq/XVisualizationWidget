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
#include "xfiledescription.h"

XFileDescription::XFileDescription(const QColor color, QString sText)
{
    g_color = color;
    g_sText = sText;

    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}

QRectF XFileDescription::boundingRect() const
{
    QRect result;
    QSize _size = QFontMetrics(font()).size(Qt::TextSingleLine, g_sText);

    qint32 nHeight = _size.height();
    qint32 nWidth = nHeight * 2 + _size.width();

    result.setHeight(nHeight);
    result.setWidth(nWidth);

    return result;
}

QPainterPath XFileDescription::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void XFileDescription::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    Q_UNUSED(pWidget)

    QColor fillColor = (pOption->state & QStyle::State_Selected) ? g_color.darker(150) : g_color;
    if (pOption->state & QStyle::State_MouseOver) {
        fillColor = fillColor.lighter(125);
    }

    QRectF rect = boundingRect();

    QRectF rectColor = QRectF(rect.left(), rect.top(), rect.height(), rect.height());
    QRectF rectText = QRectF(rect.left() + rect.height() * 2, rect.top(), rect.width() - rect.height() * 2, rect.height());

    pPainter->fillRect(rectColor, fillColor);
    pPainter->drawText(rectText, g_sText);
}

void XFileDescription::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void XFileDescription::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
    update();
}

void XFileDescription::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}
