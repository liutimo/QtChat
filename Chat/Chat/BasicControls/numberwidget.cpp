#include "numberwidget.h"
#include <QPainter>
#include <QFontMetrics>
#include <QDebug>
NumberWidget::NumberWidget(QWidget *parent) : QLabel(parent)
{
    setFixedSize(23, 23);
}

void NumberWidget::setNumber(const int num)
{
    number = num;
    update();
}

void NumberWidget::paintEvent(QPaintEvent *event)
{
    if (number !=0) {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.save();
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(255, 55, 66));
        p.drawEllipse(rect());
        p.restore();

        p.setPen(Qt::white);

        QString text = QString::number(number);

        QFontMetrics f(font());
        int w = f.width(text);
        int h = f.height();

        p.drawText(QPoint((width() - w) / 2, 16), text);
    }
    else
        QLabel::paintEvent(event);
}
