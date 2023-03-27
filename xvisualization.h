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
#ifndef XVISUALIZATION_H
#define XVISUALIZATION_H

#include <QPainter>
#include "xformats.h"

class XVisualization : public QObject
{
    Q_OBJECT
public:
    struct XAREA {
        qint64 nOffset;
        qint64 nSize;
        QString sText;
        QColor color;
    };

    struct DATA {
        qint32 nWidth;
        qint32 nHeight;
        qint32 nBlockSize;
        QList<XAREA> listRegions;
        QList<XAREA> listHighlights;
    };

    explicit XVisualization(QObject *pParent = nullptr);

    static QImage createImage(DATA *pData);
    void setData(QIODevice *pDevice, DATA *pData, XBinary::PDSTRUCT *pPdStruct);

signals:
    void errorMessage(QString sText);
    void completed(qint64 nElapsed);

public slots:
    void process();

private:
    QIODevice *g_pDevice;
    DATA *g_pData;
    XBinary::PDSTRUCT *g_pPdStruct;
};

#endif // XVISUALIZATION_H
