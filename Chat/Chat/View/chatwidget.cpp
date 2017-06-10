#include "chatwidget.h"

#include "BasicControls/pushbutton.h"
#include "BasicControls/headicon.h"

#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QTextEdit>
ChatWidget::ChatWidget(QWidget *parent) : BasicWidget(parent)
{
    widgetIcon->hide();
    widgetTitle->hide();
    btn_close->hide();

    init();

    resize(650, 500);
}

void ChatWidget::init()
{
    headIcon = new HeadIcon(this);
    headIcon->setFixedSize(35, 35);
    headIcon->setPixmap(QPixmap(":/timg (1).jpg"));

    lb_username = new QLabel(tr("Username"), this);

    btn_chat_settings = new QPushButton(this);
    btn_chat_settings->setFixedSize(28, 28);
    btn_chat_settings->setObjectName("btn_chat_settings");

    btn_chat_close = new QPushButton(this);
    btn_chat_close->setFixedSize(28, 28);
    btn_chat_close->setObjectName("btn_chat_close");
    connect(btn_chat_close, &QPushButton::clicked, this, &BasicWidget::close);

    btn_chat_min = new QPushButton(this);
    btn_chat_min->setFixedSize(28, 28);
    btn_chat_min->setObjectName("btn_chat_min");

    btn_chat_max = new QPushButton(this);
    btn_chat_max->setFixedSize(28, 28);
    btn_chat_max->setObjectName("btn_chat_max");

    textedit = new QTextEdit(this);
    textedit->setHtml("<img width=\"100px\" height=\"100px\" src=\"/home/liuzheng/Pictures/timg.jpg\"/>");


    chatinput = new ChatInput(this);
    chatinput->setObjectName("chatinput");
    connect(chatinput, &ChatInput::sendMsg, this, &ChatWidget::setMessage);
}

void ChatWidget::resizeEvent(QResizeEvent *event)
{
    headIcon->move(10, 10);

    lb_username->move(50, 12);
    lb_username->resize(width() - 172, 30);

    btn_chat_close->move(width() - 28, 0);
    btn_chat_max->move(width() - 56, 0);
    btn_chat_min->move(width() - 84, 0);
    btn_chat_settings->move(width() - 112, 0);

    textedit->resize(width(), height() - 240);
    textedit->move(0, 45);

    chatinput->resize(width() , 200);
    chatinput->move(0, height() - 200);
}

void ChatWidget::paintEvent(QPaintEvent *event)
{

}

void ChatWidget::setMessage(const QString &msg)
{
     textedit->setHtml(msg);
}
