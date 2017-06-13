#include "purecolorlabel.h"

#include <QPainter>
#include <QMouseEvent>

PureColorLabel::PureColorLabel(QWidget *parent):QLabel(parent)
{
    color = Qt::white;
    //setMouseTracking(true);
}

void PureColorLabel::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setBrush(color);
    p.setPen(Qt::NoPen);
    p.drawRect(0, 0, width(), height());

}

void PureColorLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        emit selecteColor(color);
    }
}

void PureColorLabel::mousePressEvent(QMouseEvent *ev)
{
    ev->accept();
}


void PureColorLabel::setColor(QColor c)
{
    color =c;
    update();
}

QColor PureColorLabel::getColor()
{
    return color;
}

