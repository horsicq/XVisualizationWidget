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
#ifndef XVISUALIZATION_H
#define XVISUALIZATION_H

#include <QPainter>
#include "xformats.h"
#include "xinfodb.h"

class XVisualization : public QObject {
    Q_OBJECT

public:
    enum DATAMETHOD {
        DATAMETHOD_NONE,
        DATAMETHOD_ENTROPY
    };

    struct XAREA {
        bool bIsEnabled;
        qint64 nOffset;
        qint64 nSize;
        qint64 nOffsetInBlocks;
        qint64 nSizeInBlocks;
        QString sName;
        QColor color;
    };

    struct DATA {
        qint32 nWidth;
        qint32 nHeight;
        qint32 nBlockSize;
        QColor colorBase;
        XBinary::FT fileFormat;
        XBinary::MAPMODE mapMode;
        DATAMETHOD dataMethod;
        QList<qint32> listParts;
        QList<qint32> listPartsEntropy;
        QList<XAREA> listRegions;
        QList<XAREA> listHighlights;
    };

    explicit XVisualization(QObject *pParent = nullptr);

    static QImage createImage(DATA *pData);
    static XAREA isRegionPresent(DATA *pData, qint64 nBlockIndex);
    static XAREA isHighlightPresent(DATA *pData, qint64 nBlockIndex);
    void setData(QIODevice *pDevice, DATA *pData, XBinary::PDSTRUCT *pPdStruct);

signals:
    void errorMessage(const QString &sText);
    void completed(qint64 nElapsed);

public slots:
    void handleData();
    static QColor getRegionColor(qint32 nIndex);
    static QColor getHighlightColor(qint32 nIndex);

private:
    QIODevice *g_pDevice;
    DATA *g_pData;
    XBinary::PDSTRUCT *g_pPdStruct;
};

#endif  // XVISUALIZATION_H
