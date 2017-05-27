#include "imageskinlabel.h"
#include <QPainter>
#include <QMouseEvent>
ImageSkinLabel::ImageSkinLabel(const QString &path,QWidget *parent) : QLabel(parent)
{
    setObjectName("ImageSkinLabel");

    filepath = path;


}

void ImageSkinLabel::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setPen(Qt::NoPen);
    p.drawPixmap(2, 2, width() - 4, height() - 4, QPixmap(filepath).scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void ImageSkinLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        emit selecteImage(filepath);
    }
}

void ImageSkinLabel::mousePressEvent(QMouseEvent *ev)
{
    ev->accept();
}
