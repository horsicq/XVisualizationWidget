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
#include "xvisualization.h"

XVisualization::XVisualization(QObject *pParent) : QObject(pParent)
{
    g_pDevice = nullptr;
    g_pData = nullptr;
    g_pPdStruct = nullptr;
}

QImage XVisualization::createImage(DATA *pData)
{
    // mb TODO
    QImage imageResult(QSize(pData->nBlockSize * pData->nWidth, pData->nBlockSize * pData->nHeight), QImage::Format_RGB32);
    QPainter painter(&imageResult);
    painter.setBrush(QBrush(Qt::green));
    painter.setPen(QColor(Qt::black));
    // painter.setBackground(QBrush(Qt::red));

    qint32 nIndex = 0;

    for (qint32 i = 0; i < pData->nHeight; i++) {
        for (qint32 j = 0; j < pData->nWidth; j++) {
            QRect rect(pData->nBlockSize * j, pData->nBlockSize * i, pData->nBlockSize, pData->nBlockSize);

            XAREA areaRegion = isRegionPresent(pData, nIndex);
            XAREA areaHighlight = isHighlightPresent(pData, nIndex);

            qint32 nValue = 0;

            nValue = pData->listParts.at(nIndex).nValue[pData->dataMethod];

            QColor colorBlock;

            if (areaHighlight.bIsEnabled) {
                if (areaHighlight.nSizeInBlocks == 1) {
                    colorBlock = areaHighlight.color;
                } else {
                    colorBlock = areaHighlight.color.darker(nValue);
                }
            } else if (areaRegion.bIsEnabled) {
                colorBlock = areaRegion.color.darker(nValue);
            } else {
                colorBlock = pData->colorBase.darker(nValue);
            }

            painter.fillRect(rect, colorBlock);
            //            painter.drawRect(rect);

            nIndex++;
        }
    }

    //    painter.fillRect(QRectF(0,0,20,20),Qt::green);
    //    painter.fillRect(QRectF(100,100,200,100),Qt::white);

    return imageResult;
}

XVisualization::XAREA XVisualization::isRegionPresent(DATA *pData, qint64 nBlockIndex)
{
    XAREA result = {};

    qint32 nCount = pData->listRegions.count();

    for (qint32 i = 0; i < nCount; i++) {
        if (pData->listRegions.at(i).bIsEnabled) {
            if ((nBlockIndex >= pData->listRegions.at(i).nOffsetInBlocks) &&
                (nBlockIndex < (pData->listRegions.at(i).nOffsetInBlocks + pData->listRegions.at(i).nSizeInBlocks))) {
                result = pData->listRegions.at(i);
                break;
            }
        }
    }

    return result;
}

XVisualization::XAREA XVisualization::isHighlightPresent(DATA *pData, qint64 nBlockIndex)
{
    XAREA result = {};

    qint32 nCount = pData->listHighlights.count();

    for (qint32 i = 0; i < nCount; i++) {
        if (pData->listHighlights.at(i).bIsEnabled) {
            if ((nBlockIndex >= pData->listHighlights.at(i).nOffsetInBlocks) &&
                (nBlockIndex < (pData->listHighlights.at(i).nOffsetInBlocks + pData->listHighlights.at(i).nSizeInBlocks))) {
                result = pData->listHighlights.at(i);
                break;
            }
        }
    }

    return result;
}

void XVisualization::setData(QIODevice *pDevice, DATA *pData, XBinary::PDSTRUCT *pPdStruct)
{
    g_pDevice = pDevice;
    g_pData = pData;
    g_pPdStruct = pPdStruct;
}

QMap<quint64, QString> XVisualization::getMethodFlags()
{
    QMap<quint64, QString> mapResult;

    mapResult.insert(1 << XVisualization::DATAMETHOD_NONE, methodToString(XVisualization::DATAMETHOD_NONE));
    mapResult.insert(1 << XVisualization::DATAMETHOD_ENTROPY, methodToString(XVisualization::DATAMETHOD_ENTROPY));
    mapResult.insert(1 << XVisualization::DATAMETHOD_GRADIENT, methodToString(XVisualization::DATAMETHOD_GRADIENT));
    mapResult.insert(1 << XVisualization::DATAMETHOD_ZEROS, methodToString(XVisualization::DATAMETHOD_ZEROS));
    mapResult.insert(1 << XVisualization::DATAMETHOD_ZEROS_GRADIENT, methodToString(XVisualization::DATAMETHOD_ZEROS_GRADIENT));
    mapResult.insert(1 << XVisualization::DATAMETHOD_TEXT, methodToString(XVisualization::DATAMETHOD_TEXT));
    mapResult.insert(1 << XVisualization::DATAMETHOD_TEXT_GRADIENT, methodToString(XVisualization::DATAMETHOD_TEXT_GRADIENT));

    return mapResult;
}

quint64 XVisualization::getDefaultMethodFlag()
{
    quint64 nResult = 0;

    nResult |= (1 << XVisualization::DATAMETHOD_NONE);
    nResult |= (1 << XVisualization::DATAMETHOD_ENTROPY);
    nResult |= (1 << XVisualization::DATAMETHOD_ZEROS);
    nResult |= (1 << XVisualization::DATAMETHOD_ZEROS_GRADIENT);

    return nResult;
}

QString XVisualization::methodToString(DATAMETHOD dataMethod)
{
    QString sResult;

    switch (dataMethod) {
        case DATAMETHOD_NONE: sResult = tr("None"); break;
        case DATAMETHOD_ENTROPY: sResult = tr("Entropy"); break;
        case DATAMETHOD_GRADIENT: sResult = tr("Gradient"); break;
        case DATAMETHOD_ZEROS: sResult = tr("Zeros"); break;
        case DATAMETHOD_ZEROS_GRADIENT: sResult = QString("%1(%2)").arg(tr("Zeros"), tr("Gradient")); break;
        case DATAMETHOD_TEXT: sResult = tr("Text"); break;
        case DATAMETHOD_TEXT_GRADIENT: sResult = QString("%1(%2)").arg(tr("Text"), tr("Gradient")); break;
        default: sResult = tr("Unknown");
    }

    return sResult;
}

QList<XVisualization::DATAMETHOD> XVisualization::getMethodsListFromFlags(quint64 nMethodFlags)
{
    QList<DATAMETHOD> listResult;

    for (qint32 i = 0; i < __SIZE_DATAMETHOD; i++) {
        if (nMethodFlags & ((quint64)1 << i)) {
            listResult.append((DATAMETHOD)i);
        }
    }

    return listResult;
}

void XVisualization::handleData()
{
    QElapsedTimer scanTimer;
    scanTimer.start();

    g_pData->listParts.clear();
    g_pData->listRegions.clear();
    g_pData->listHighlights.clear();

    qint32 _nFreeIndex = XBinary::getFreeIndex(g_pPdStruct);

    XBinary binary(g_pDevice);

    qint64 nFileSize = binary.getSize();
    qint32 nNumberOfBlocks = g_pData->nHeight * g_pData->nWidth;
    double dFileBlockSize = (double)nFileSize / (nNumberOfBlocks);
    g_pData->nFileBlockSize = dFileBlockSize;

    bool bEntropy = g_pData->nMethodsFlags & (1 << DATAMETHOD_ENTROPY);
    bool bGradient = g_pData->nMethodsFlags & (1 << DATAMETHOD_GRADIENT);
    bool bZeros = g_pData->nMethodsFlags & (1 << DATAMETHOD_ZEROS);
    bool bZerosGradient = g_pData->nMethodsFlags & (1 << DATAMETHOD_ZEROS_GRADIENT);
    bool bText = g_pData->nMethodsFlags & (1 << DATAMETHOD_TEXT);
    bool bTextGradient = g_pData->nMethodsFlags & (1 << DATAMETHOD_TEXT_GRADIENT);

    XBinary::setPdStructInit(g_pPdStruct, _nFreeIndex, nNumberOfBlocks);

    for (qint32 i = 0; (i < nNumberOfBlocks) && (!(g_pPdStruct->bIsStop)); i++) {
        PART part = {};
        part.nOffset = i * dFileBlockSize;
        part.nValue[DATAMETHOD_NONE] = 100;

        if (bEntropy) {
            part.dEntropy = binary.getBinaryStatus(XBinary::BSTATUS_ENTROPY, part.nOffset, g_pData->nFileBlockSize, g_pPdStruct);
            part.nValue[DATAMETHOD_ENTROPY] = 100 + (200.0 * part.dEntropy) / 8.0;
        }

        if (bGradient) {
            part.dGradient = binary.getBinaryStatus(XBinary::BSTATUS_GRADIENT, part.nOffset, g_pData->nFileBlockSize, g_pPdStruct);
            part.nValue[DATAMETHOD_GRADIENT] = 100 + (200.0 * part.dGradient);
        }

        if (bZeros || bZerosGradient) {
            part.dZeros = binary.getBinaryStatus(XBinary::BSTATUS_ZEROS, part.nOffset, g_pData->nFileBlockSize, g_pPdStruct);
            part.nValue[DATAMETHOD_ZEROS_GRADIENT] = 100 + (200.0 * part.dZeros);

            if (part.dZeros == 1.0) {
                part.nValue[DATAMETHOD_ZEROS] = 300;
            } else {
                part.nValue[DATAMETHOD_ZEROS] = 100;
            }
        }

        if (bText || bTextGradient) {
            part.dText = binary.getBinaryStatus(XBinary::BSTATUS_TEXT, part.nOffset, g_pData->nFileBlockSize, g_pPdStruct);
            part.nValue[DATAMETHOD_TEXT_GRADIENT] = 100 + (200.0 * part.dText);

            if (part.dText == 1.0) {
                part.nValue[DATAMETHOD_TEXT] = 300;
            } else {
                part.nValue[DATAMETHOD_TEXT] = 100;
            }
        }

        g_pData->listParts.append(part);
        XBinary::setPdStructCurrent(g_pPdStruct, _nFreeIndex, i);
    }

    XBinary::_MEMORY_MAP memoryMap = XFormats::getMemoryMap(g_pData->fileFormat, g_pData->mapMode, g_pDevice, false, -1, g_pPdStruct);

    {
        XBinary::HLOPTIONS hlOptions = {};
        hlOptions.bRegions = true;

        QList<XBinary::HREGION> listHRegions = XFormats::getHighlights(g_pData->fileFormat, g_pDevice, &memoryMap, hlOptions, false, -1, g_pPdStruct);

        qint32 nNumberOfRecords = listHRegions.count();

        for (qint32 i = 0; i < nNumberOfRecords; i++) {
            if (listHRegions.at(i).nOffset != -1) {
                double dSizeInBlocks = (double)listHRegions.at(i).nSize / dFileBlockSize;

                XAREA xarea = {};
                xarea.bIsEnabled = true;
                xarea.color = getRegionColor(i);
                xarea.nOffset = listHRegions.at(i).nOffset;
                xarea.nSize = listHRegions.at(i).nSize;
                xarea.nOffsetInBlocks = (double)listHRegions.at(i).nOffset / dFileBlockSize;
                xarea.nSizeInBlocks = dSizeInBlocks;
                xarea.sName = listHRegions.at(i).sName;

                if (dSizeInBlocks > xarea.nSizeInBlocks) {
                    xarea.nSizeInBlocks++;
                }

                g_pData->listRegions.append(xarea);
            }
        }
    }
    {
        XBinary::HLOPTIONS hlOptions = {};
        hlOptions.bHighlights = true;

        QList<XBinary::HREGION> listHighlights = XFormats::getHighlights(g_pData->fileFormat, g_pDevice, &memoryMap, hlOptions, false, -1, g_pPdStruct);

        qint32 nNumberOfRecords = listHighlights.count();

        for (qint32 i = 0; i < nNumberOfRecords; i++) {
            if (listHighlights.at(i).nOffset != -1) {
                double dSizeInBlocks = (double)listHighlights.at(i).nSize / dFileBlockSize;

                XAREA xarea = {};
                xarea.bIsEnabled = true;
                xarea.color = getHighlightColor(i);
                xarea.nOffset = listHighlights.at(i).nOffset;
                xarea.nSize = listHighlights.at(i).nSize;
                xarea.nOffsetInBlocks = (double)listHighlights.at(i).nOffset / dFileBlockSize;
                xarea.nSizeInBlocks = dSizeInBlocks;
                xarea.sName = listHighlights.at(i).sName;

                if (dSizeInBlocks > xarea.nSizeInBlocks) {
                    xarea.nSizeInBlocks++;
                }

                g_pData->listHighlights.append(xarea);
            }
        }
    }

    XBinary::setPdStructFinished(g_pPdStruct, _nFreeIndex);

    emit completed(scanTimer.elapsed());
}

QColor XVisualization::getRegionColor(qint32 nIndex)
{
    QColor result = Qt::gray;

    nIndex = nIndex % 11;

    switch (nIndex) {
        case 0: result = XInfoDB::stringToColor("#808000"); break;
        case 1: result = XInfoDB::stringToColor("#008b8b"); break;
        case 2: result = XInfoDB::stringToColor("#cd853f"); break;
        case 3: result = XInfoDB::stringToColor("#9acd32"); break;
        case 4: result = XInfoDB::stringToColor("#8fbc8f"); break;
        case 5: result = XInfoDB::stringToColor("#b03060"); break;
        case 6: result = XInfoDB::stringToColor("#f08080"); break;
        case 7: result = XInfoDB::stringToColor("#da70d6"); break;
        case 8: result = XInfoDB::stringToColor("#f0e68c"); break;
        case 9: result = XInfoDB::stringToColor("#87ceeb"); break;
        case 10: result = XInfoDB::stringToColor("#7b68ee"); break;
        case 11: result = XInfoDB::stringToColor("#fff5ee"); break;
        default: result = Qt::gray;
    }

    return result;
}

QColor XVisualization::getHighlightColor(qint32 nIndex)
{
    QColor result = Qt::gray;

    nIndex %= 7;

    switch (nIndex) {
        case 0: result = XInfoDB::stringToColor("#dc143c"); break;
        case 1: result = XInfoDB::stringToColor("#0000ff"); break;
        case 2: result = XInfoDB::stringToColor("#a020f0"); break;
        case 3: result = XInfoDB::stringToColor("#00ff00"); break;
        case 4: result = XInfoDB::stringToColor("#7f007f"); break;
        case 5: result = XInfoDB::stringToColor("#ff4500"); break;
        case 6: result = XInfoDB::stringToColor("#006400"); break;
        case 7: result = XInfoDB::stringToColor("##ff1493"); break;
        default: result = Qt::gray;
    }

    return result;
}
