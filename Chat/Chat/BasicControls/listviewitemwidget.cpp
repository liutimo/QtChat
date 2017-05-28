#include "listviewitemwidget.h"

#include "headicon.h"
#include <QPixmap>
#include <QLabel>

ListViewItemWidget::ListViewItemWidget(QWidget *parent) : QWidget(parent)
{
    init();
    resize(200, 40);
}

void ListViewItemWidget::init()
{
    m_headicon = new HeadIcon(this);
    m_headicon->resize(30, 30);
    m_headicon->setPixmap(QPixmap(":/timg (1).jpg"));
    m_nickname = new QLabel(this);
    m_nickname->setTextFormat(Qt::RichText);

    m_nickname->setText("<b>我是昵称</b> 我是个性签名啊！！！！");
}

void ListViewItemWidget::resizeEvent(QResizeEvent *event)
{
    m_headicon->move(5, (height() - 30) / 2);

    m_nickname->resize(width() - 80, height() - 10);
    m_nickname->move(40, 5);
}
