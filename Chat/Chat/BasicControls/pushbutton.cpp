#include "pushbutton.h"

#include <QPainter>

PushButton::PushButton(QWidget *parent) : QPushButton(parent)
{
    backgroundcolor = Qt::white;
}

void PushButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

   // p.setBrush(backgroundcolor);
   // p.drawRect(0, 0, width(), height());


}


void PushButton::setBackgroundColor(QColor color)
{
    backgroundcolor = color;
    update();
}
