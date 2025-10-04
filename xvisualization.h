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
#ifndef XVISUALIZATION_H
#define XVISUALIZATION_H

#include <QPainter>
#include "xformats.h"
#include "xinfodb.h"
#include "xthreadobject.h"

class XVisualization : public XThreadObject {
    Q_OBJECT

public:
    enum DATAMETHOD {
        DATAMETHOD_NONE,
        DATAMETHOD_ENTROPY,
        DATAMETHOD_GRADIENT,
        DATAMETHOD_ZEROS,
        DATAMETHOD_ZEROS_GRADIENT,
        DATAMETHOD_TEXT,
        DATAMETHOD_TEXT_GRADIENT,
        __SIZE_DATAMETHOD
        // TODO code
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

    struct PART {
        qint16 nValue[__SIZE_DATAMETHOD];
        qint64 nOffset;
        double dEntropy;
        double dZeros;
        double dGradient;
        double dText;
    };

    struct DATA {
        qint32 nWidth;
        qint32 nHeight;
        qint32 nBlockSize;
        qint32 nFileBlockSize;
        QColor colorBase;
        XBinary::FT fileFormat;
        XBinary::MAPMODE mapMode;
        quint64 nMethodsFlags;
        DATAMETHOD dataMethod;
        QList<PART> listParts;
        QList<XAREA> listRegions;
        QList<XAREA> listHighlights;
    };

    explicit XVisualization(QObject *pParent = nullptr);

    static QImage createImage(DATA *pData);
    static XAREA isRegionPresent(DATA *pData, qint64 nBlockIndex);
    static XAREA isHighlightPresent(DATA *pData, qint64 nBlockIndex);
    void setData(QIODevice *pDevice, DATA *pData, XBinary::PDSTRUCT *pPdStruct);
    static QMap<quint64, QString> getMethodFlags();
    static quint64 getDefaultMethodFlag();
    static QString methodToString(DATAMETHOD dataMethod);
    static QList<DATAMETHOD> getMethodsListFromFlags(quint64 nMethodFlags);

public slots:
    void process();
    static QColor getRegionColor(qint32 nIndex);
    static QColor getHighlightColor(qint32 nIndex);

private:
    QIODevice *m_pDevice;
    DATA *m_pData;
    XBinary::PDSTRUCT *m_pPdStruct;
};

#endif  // XVISUALIZATION_H
