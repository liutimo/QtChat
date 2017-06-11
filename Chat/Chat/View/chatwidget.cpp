#include "chatwidget.h"

#include "BasicControls/pushbutton.h"
#include "BasicControls/headicon.h"

#include <NetWork/connecttoserver.h>

#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QTextEdit>
#include <QDebug>
#include <QTextObject>

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
    qDebug() << msg << " size=" << msg.size() << " length="<<msg.length();
    char *buf = new char[sizeof(RequestForwordMessageMsg) + msg.length()];

    RequestForwordMessageMsg *rmsg = (RequestForwordMessageMsg*)buf;
    strcpy(rmsg->friendid, "123457");
    rmsg->length = msg.length();
    memcpy(rmsg->message, msg.toUtf8().data(), msg.length());

    qDebug() << rmsg->message;

    ConnectToServer::getInstance()->sendRequestForwordMessageMsg(rmsg);

    delete []buf;

    textedit->setText(msg);
    QTextBlock::iterator it;

    QTextBlock block = textedit->document()->begin();
//    while(block.isValid())
//    {
//        for(it = block.begin(); !(it.atEnd());)
//        {
//            QTextFragment currentFragment = it.fragment();
//            QTextImageFormat newImageFormat = currentFragment.charFormat().toImageFormat();

//            if(newImageFormat.isValid())
//            {
//                ++it;
//                qDebug() << "image";
//                continue;
//            }
//            if(currentFragment.isValid())
//            {
//                ++it;
//                int pos = currentFragment.position();
//                QString strText = currentFragment.text();
//                for(int i = 0; i < strText.length(); ++i)
//                {
//                    QTextCharFormat fmt;
//                    fmt.setForeground(QColor(qrand() % 255, qrand() % 255, qrand() % 255));
//                    QTextCursor helper = textedit->textCursor();
//                    helper.setPosition(pos++);
//                    helper.setPosition(pos, QTextCursor::KeepAnchor);
//                    helper.setCharFormat(fmt);
//                }
//            }

//        }
//        block = block.next();
//    }
}
