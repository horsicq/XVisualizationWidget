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

XVisualization::XVisualization(QObject *pParent) : QObject(pParent)
{
}

QImage XVisualization::createImage(DATA *pData)
{
    QImage imageResult(QSize(pData->nBlockSize * pData->nWidth, pData->nBlockSize * pData->nHeight), QImage::Format_RGB32);
    QPainter painter(&imageResult);
    painter.setBrush(QBrush(Qt::green));
    painter.setPen(QColor(Qt::black));
    // painter.setBackground(QBrush(Qt::red));

    qint32 nIndex = 0;

    for (qint32 i = 0; i < pData->nHeight; i++) {
        for (qint32 j = 0; j < pData->nWidth; j++) {
            QRect rect(pData->nBlockSize * j, pData->nBlockSize * i, pData->nBlockSize, pData->nBlockSize);

            qint32 nValue = pData->listParts.at(nIndex);

            QColor colorBlock = pData->colorBase.darker(nValue);

            painter.fillRect(rect, colorBlock);
            //            painter.drawRect(rect);

            nIndex++;
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

    g_pData->listParts.clear();

    qint32 _nFreeIndex = XBinary::getFreeIndex(g_pPdStruct);

    XBinary binary(g_pDevice);

    qint64 nFileSize = binary.getSize();
    qint32 nNumberOfBlocks = g_pData->nHeight * g_pData->nWidth;
    qint64 nFileBlockSize = nFileSize / (nNumberOfBlocks);

    XBinary::setPdStructInit(g_pPdStruct, _nFreeIndex, nNumberOfBlocks);

    if (g_pData->dataMethod == DATAMETHOD_NONE) {
        for (qint32 i = 0; (i < nNumberOfBlocks) && (!(g_pPdStruct->bIsStop)); i++) {
            g_pData->listParts.append(100);
        }
    } else if (g_pData->dataMethod == DATAMETHOD_ENTROPY) {
        for (qint32 i = 0; (i < nNumberOfBlocks) && (!(g_pPdStruct->bIsStop)); i++) {
            double dValue = 0;
            dValue = binary.getEntropy(i * nFileBlockSize, nFileBlockSize, g_pPdStruct);
            qint32 nValue = 100 + (200 * dValue) / 8;
            g_pData->listParts.append(nValue);
        }
    }

    // TODO

    XBinary::setPdStructFinished(g_pPdStruct, _nFreeIndex);

    emit completed(scanTimer.elapsed());
}
