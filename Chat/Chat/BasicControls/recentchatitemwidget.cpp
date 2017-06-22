#include "recentchatitemwidget.h"
#include "DataBase/database.h"
#include "NetWork/httpconnect.h"
#include "Setting/rwsetting.h"
#include "NetWork/connecttoserver.h"
#include "showinfowidget.h"
#include "View/chatwidget.h"
#include "../allvariable.h"
#include "headicon.h"

#include <QPixmap>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QSettings>

RecentChatItemWidget::RecentChatItemWidget(QWidget *parent) : QWidget(parent)
{
    init();
    resize(200, 50);
}

void RecentChatItemWidget::init()
{
    m_headicon = new HeadIcon(this);
    m_headicon->resize(30, 30);

    m_nickname = new QLabel(this);
    m_nickname->setTextFormat(Qt::RichText);

    m_nickname->setText("<b>我是昵称</b> 我是个性签名啊！！！！");

    m_chatmessage = new QLabel(this);

    personMenu = new QMenu();

    QAction *sendMsg = new QAction(QIcon(":/Resource/mainwidget/sendmsg.png"), "发送即时消息", this);
    connect(sendMsg, &QAction::triggered, this, &RecentChatItemWidget::listWidgetMenuTriggered);

    QAction *del = new QAction(QIcon(":/Resource/mainwidget/deluser.png"), "删除会话", this);
    connect(del, &QAction::triggered, this, [this](){
        QSettings *setting = RWSetting::getInstance()->getSetting();
        QStringList us = setting->value("RecentlyChat").toStringList();
        for(QStringList::iterator iter = us.begin(); iter != us.end(); ++iter)
        {
            if(*iter == userid)
            {
                us.erase(iter);
                break;
            }
        }
        setting->setValue("RecentlyChat", us);
        emit delOneItem();
    });

    personMenu->addAction(sendMsg);
    personMenu->addAction(del);
}

void RecentChatItemWidget::resizeEvent(QResizeEvent *event)
{
    m_headicon->move(5, (height() - 30) / 2);

    m_nickname->resize(width() - 80, 20);
    m_nickname->move(40, 5);

    m_chatmessage->resize(width() - 80, 20);
    m_chatmessage->move(40, 30);

}
void RecentChatItemWidget::setUserinfo(const QString &userid, const QString &username, const QString &signature)
{
    this->userid = userid;
    this->username = username;
    m_nickname->setText(QString("<em style=\"font-size:14px; color:#123;\">%1</em> <span style=\"font-size:12px; color:#123;\">%2</span>").arg(username, signature));
}

QString RecentChatItemWidget::getUsername()
{
    return username;
}

QString RecentChatItemWidget::getUserid()
{
    return userid;
}

void RecentChatItemWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QWidget::contextMenuEvent(event);           //调用基类事件
    personMenu->exec(QCursor::pos());
}

void RecentChatItemWidget::listWidgetMenuTriggered()
{
    QMap<QString, ChatWidget*>& chatwidgets = AllVariable::getChatWidgetMap();

    ChatWidget *chat = chatwidgets.value(userid);

    if (chat == NULL)
    {
        chat = new ChatWidget();
        chatwidgets.insert(userid, chat);
        chat->setIcon(imagePath);
    }

    chat->setUserName(username);
    chat->setUserid(userid);
    chat->show();
}

void RecentChatItemWidget::setImage(const QString& url)
{
    HttpConnect *http = new HttpConnect();

    if(QFile(QUrl(url).fileName()).exists())
    {
        m_headicon->setPixmap(QPixmap(QUrl(url).fileName()));
    }
    else
    {
        http->loadFileFormUrl(url);
        connect(http, &HttpConnect::loadCompleted, this, [this, http](){
            imagePath = http->getFilePath();
            m_headicon->setPixmap(QPixmap(imagePath));
        });
    }

}

void RecentChatItemWidget::moveFriendTo()
{
    QAction *s = static_cast<QAction*>(sender());
    DataBase::getInstance()->moveFriendToGroup(userid, s->text());
    ConnectToServer::getInstance()->sendRequestMoveFriendToGroup(userid, s->text());
    emit updateListWidget();
}

void RecentChatItemWidget::setMessage(const QString &message)
{
    qDebug() << message;
    QString p("</em><br/>(.*)</html>");
    QRegExp rx(p);
    rx.indexIn(message);
    QString msg = rx.cap(1);
    qDebug() << msg;
    m_chatmessage->setText(msg);
}
