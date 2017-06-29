#include "bubble.h"

#include <QLabel>
#include <QPainter>
#include <QDebug>

Bubble::Bubble(QWidget *parent) : QLabel(parent)
{
    this->setStyleSheet("padding: 10px;");
    this->setMaximumWidth(400);
}


Bubble::~Bubble()
{
}

void Bubble::setBubbleOrientatio(BubbleOrientation _orientation)
{
    orientation = _orientation;
}

/*
    实现文本自动换行
*/
void  Bubble::setText(const QString &text)
{

    QFontMetrics fm(font());

    int textWidth = fm.width(text);


    if (textWidth > 380)
    {
        QString _text;
        QString line;
        for (int i = 0; i < text.size(); ++i)
        {
            _text.append(text[i]);
            line.append(text[i]);
            if (fm.width(line) > 370)
            {
                _text.append('\r');
                line.clear();
            }
        }
        QLabel::setText(_text);
    }
    else
        QLabel::setText(text);
    update();
}

void Bubble::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::red);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(222, 222, 217));

    if (orientation == BubbleOrientation::Left)
    {
        p.drawRoundedRect(8, 0, width() - 10, height(), 5, 5);
        QPoint point[3] = { QPoint(8, 10), QPoint(8, 20), QPoint(0, 15)};
        p.drawPolygon(point, 3);

    }
    else if (orientation == BubbleOrientation::Right)
    {
        p.drawRoundedRect(0, 0, width() - 8, height(), 5, 5);
        QPoint point[3] = { QPoint(width() - 8, 10), QPoint(width() - 8, 20), QPoint(width(), 15) };
        p.drawPolygon(point, 3);
    }

    QLabel::paintEvent(event);
}
