#include "xvisualizationwidget.h"
#include "ui_xvisualizationwidget.h"

XVisualizationWidget::XVisualizationWidget(QWidget *pParent) :
    QWidget(pParent),
    ui(new Ui::XVisualizationWidget)
{
    ui->setupUi(this);

    g_pDevice = nullptr;
}

XVisualizationWidget::~XVisualizationWidget()
{
    delete ui;
}

QImage XVisualizationWidget::createImage()
{
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

//            painter.fillRect(rect, Qt::green);
            painter.drawRect(rect);
        }
    }

//    painter.fillRect(QRectF(0,0,20,20),Qt::green);
//    painter.fillRect(QRectF(100,100,200,100),Qt::white);

    return imageResult;
}

void XVisualizationWidget::setData(QIODevice *pDevice, XBinary::FT fileType, bool bAuto)
{
    this->g_pDevice = pDevice;

    XFormats::setFileTypeComboBox(fileType, g_pDevice, ui->comboBoxType);

    if (bAuto) {
        reload();
    }
}

void XVisualizationWidget::reload()
{
    // TODO
}
