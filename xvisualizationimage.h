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
#ifndef XVISUALIZATIONIMAGE_H
#define XVISUALIZATIONIMAGE_H

#include <QFrame>
#include <QPaintEvent>
#include <QMouseEvent>
#include "xvisualization.h"
#include "xshortcutswidget.h"

class XVisualizationImage : public XShortcutsWidget {
    Q_OBJECT

public:
    explicit XVisualizationImage(QWidget *pParent = nullptr);

    void setData(XVisualization::DATA *pData);
    void clear();
    QPixmap getPixmap();

    virtual void adjustView();
    virtual void reloadData(bool bSaveSelection);

private:
    void adjust();
    void _clear();

private slots:
    void customContextMenu(const QPoint &pos);

protected:
    virtual void paintEvent(QPaintEvent *pEvent) override;
    virtual void mousePressEvent(QMouseEvent *pEvent) override;
    virtual void resizeEvent(QResizeEvent *pEvent) override;

protected:
    virtual void registerShortcuts(bool bState);

private:
    XVisualization::DATA g_data;
    QPixmap g_pixmap;
    qint32 g_nY;
    qint32 g_nX;
    qint32 g_nCurrentIndex;
};

#endif  // XVISUALIZATIONIMAGE_H
