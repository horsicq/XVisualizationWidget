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
#include "xvisualization.h"

XVisualization::XVisualization(QObject *pParent)
    : QObject(pParent)
{

}

QImage XVisualization::createImage(DATA *pData)
{
//    qint32 nBlockSize = 3;
//    qint32 nWidth = 100;
//    qint32 nHeight = 200;

    qint32 nBlockSize = 3;
    qint32 nWidth = 100;
    qint32 nHeight = 200;

    QImage imageResult(QSize(nBlockSize * nWidth, nBlockSize * nHeight), QImage::Format_RGB32);
    QPainter painter(&imageResult);
    painter.setBrush(QBrush(Qt::green));
    painter.setPen(QColor(Qt::black));
    //painter.setBackground(QBrush(Qt::red));

    for (qint32 i = 0; i < nWidth; i++) {
        for (qint32 j = 0; j < nHeight; j++) {
            QRect rect(nBlockSize * i, nBlockSize * j, nBlockSize, nBlockSize);

            painter.fillRect(rect, Qt::gray);
//            painter.drawRect(rect);
        }
    }

//    painter.fillRect(QRectF(0,0,20,20),Qt::green);
//    painter.fillRect(QRectF(100,100,200,100),Qt::white);

    return imageResult;
}

void XVisualization::setData(QIODevice *pDevice, DATA *pData, XBinary::PDSTRUCT *pPdStruct)
{
    g_pDevice = pDevice;
    g_pData = pData;
    g_pPdStruct = pPdStruct;
}

void XVisualization::process()
{
    QElapsedTimer scanTimer;
    scanTimer.start();

    qint32 _nFreeIndex = XBinary::getFreeIndex(g_pPdStruct);

    XBinary::setPdStructFinished(g_pPdStruct, _nFreeIndex);

    emit completed(scanTimer.elapsed());
}
