#include "messageitemwidget.h"
#include "numberwidget.h"
#include <QMouseEvent>
#include <QLabel>
#include <QDebug>
MessageItemWidget::MessageItemWidget(QWidget *parent) : QWidget(parent)
{
    init();
}

void MessageItemWidget::init()
{
    headicon = new QLabel(this);
    headicon->setFixedSize(32, 32);
    friendname = new QLabel(this);
    numberWidget = new NumberWidget(this);
}

void MessageItemWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        qDebug() << "s";
        ;
    QWidget::mousePressEvent(event);
}

void MessageItemWidget::resizeEvent(QResizeEvent *event)
{
    headicon->move(5, (height() - 32) / 2);

    friendname->resize(width() - 72, 32);
    friendname->move(40, (height() - friendname->height()) / 2);

    numberWidget->move(width() - 40, (height() - numberWidget->height()) / 2);

}

void MessageItemWidget::setIcon(const QString &iconpath)
{
    headicon->setPixmap(QPixmap(iconpath).scaled(32, 32, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void MessageItemWidget::setFriendName(const QString &_friendname)
{
    friendname->setText(_friendname);
}

void MessageItemWidget::setNumber(int number)
{
    numberWidget->setNumber(number);
}
