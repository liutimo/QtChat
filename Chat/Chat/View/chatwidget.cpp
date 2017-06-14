#include "chatwidget.h"
#include "../allvariable.h"
#include "BasicControls/pushbutton.h"
#include "BasicControls/headicon.h"

#include <NetWork/connecttoserver.h>

#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QTextEdit>
#include <QDebug>
#include <QTextObject>
#include <QDateTime>
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
    textedit->setReadOnly(true);


    chatinput = new ChatInput(this);
    chatinput->setObjectName("chatinput");
    connect(chatinput, &ChatInput::sendMsg, this, &ChatWidget::setMessage);
    connect(ConnectToServer::getInstance(), &ConnectToServer::receivedMessage, this, &ChatWidget::showMessage);

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

    if(msg.isEmpty())
        return;

    QStringList fontinfo =  chatinput->getFontInfo();

    char *buf = new char[sizeof(RequestForwordMessageMsg) + msg.length()];
    RequestForwordMessageMsg *rmsg = (RequestForwordMessageMsg*)buf;
    strcpy(rmsg->friendid, userid.toUtf8().data());
    strcpy(rmsg->font, fontinfo.at(0).toUtf8().data());
    strcpy(rmsg->size, fontinfo.at(1).toUtf8().data());
    strcpy(rmsg->color, fontinfo.at(2).toUtf8().data());

    rmsg->length = msg.length();
    memcpy(rmsg->message, msg.toUtf8().data(), msg.length());
    ConnectToServer::getInstance()->sendRequestForwordMessageMsg(rmsg);
    delete []buf;


    QString html = QString("<html><b style=\"color:green; font-size:16px;\">%1</b> <em style=\"color:gray; font-size:12px;\">%2</em>"
                   "<br/>%3"
                   "<br/></html>").arg(AllVariable::getLoginUserName(), QDateTime::currentDateTime().toString("h:m:s ap"), msg);

    textedit->append(html);
}

void ChatWidget::showMessage(const QString &msg, const QString &color, const QString &size, const QString &family)
{
    QString html = QString("<html><b style=\"color:green; font-size:16px;\">%1</b> <em style=\"color:gray; font-size:12px;\">%2</em>"
                   "<br/><span style=\"color:%3; font-size:%4px;font-family:%5;\">%6</span>"
                   "<br/></html>").arg(lb_username->text(), QDateTime::currentDateTime().toString("h:m:s ap"), color, size, family, msg);

    qDebug() << color << " " << size;
    textedit->append(html);
}

void ChatWidget::setUserName(const QString &username)
{
    lb_username->setText(username);
}

void ChatWidget::setUserid(const QString &userid)
{
    this->userid = userid;
}
