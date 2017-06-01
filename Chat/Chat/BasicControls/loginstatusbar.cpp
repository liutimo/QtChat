#include "loginstatusbar.h"

#include <QToolButton>
#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>
#include <QDebug>
#include <QPainter>
LoginStatusBar::LoginStatusBar(QWidget *parent) : QWidget(parent)
{
    init();
}


void LoginStatusBar::init()
{
    btn_hide = new QToolButton(this);
    btn_hide->setFixedSize(18, 18);
    btn_hide->setObjectName("btn_hide");

    lb_showinfo = new QLabel(this);
    lb_showinfo->setFixedHeight(18);

    lb_showicon = new QLabel(this);
    lb_showicon->setFixedSize(16, 16);
    lb_showicon->setPixmap(QPixmap(":/Resource/loginwidget/i.png"));


    connect(btn_hide, &QToolButton::clicked, this, &LoginStatusBar::btn_hide_oncliked);
}

void LoginStatusBar::resizeEvent(QResizeEvent *event)
{
    btn_hide->move(width() - 22, (height() - btn_hide->height()) / 2);

    lb_showicon->move(2, (height() - 14) / 2);

    lb_showinfo->move(18, (height() - 18) / 2);
    lb_showinfo->resize(width() - 40, height());

}


void LoginStatusBar::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    p.setBrush(QColor(249, 245, 214));
    p.setPen(Qt::NoPen);

    p.drawRect(rect());
}

void LoginStatusBar::setInfo(const QString &info)
{
    lb_showinfo->setText(info);
}

void LoginStatusBar::btn_hide_oncliked()
{
    emit hide_status();
}
