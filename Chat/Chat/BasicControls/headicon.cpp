#include "headicon.h"

#include <QPainter>
#include <QPainterPath>

HeadIcon::HeadIcon(QWidget *parent) : QLabel(parent)
{
    //
}

void HeadIcon::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);


    QPainterPath path;
    path.addEllipse(2, 2, width() - 4, height() -4 );

    p.setClipPath(path);
    p.clipPath();

    p.setPen(Qt::NoPen);
    p.drawPixmap(rect(), pixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void HeadIcon::setPixmap(const QPixmap &p)
{
    pixmap = p;
}
