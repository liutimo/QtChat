#include "listviewitemwidget.h"
#include "DataBase/database.h"
#include "NetWork/httpconnect.h"
#include "NetWork/connecttoserver.h"
#include "Setting/rwsetting.h"
#include <QSettings>
#include "showinfowidget.h"
#include "View/chatwidget.h"
#include "../allvariable.h"
#include "headicon.h"
#include <QPixmap>
#include <QLabel>
#include <QMenu>
#include <QAction>

ListViewItemWidget::ListViewItemWidget(QWidget *parent) : QWidget(parent)
{
    init();
    resize(300, 50);
}

void ListViewItemWidget::init()
{
    m_headicon = new HeadIcon(this);
    m_headicon->resize(30, 30);

    m_nickname = new QLabel(this);
    m_nickname->setTextFormat(Qt::RichText);

    m_nickname->setText("<b>我是昵称</b> 我是个性签名啊！！！！");
    m_nickname->move(40, 5);
    m_nickname->resize(width() - 80, height() - 10);

    m_status = new QLabel(this);
    m_status->setFixedSize(32, 32);
    m_status->move(width() - 50, 5);
    m_status->setPixmap(QPixmap(":/Resource/status/offline.png").scaled(32,32, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    personMenu = new QMenu();
    movetoMenu = new QMenu("移动好友至", this);
    movetoMenu->setIcon(QIcon(":/Resource/mainwidget/moveuser.png"));
    for(QString group : DataBase::getInstance()->getGroup())
    {
        QAction *action = new QAction(group, this);
        movetoMenu->addAction(action);
        connect(action, &QAction::triggered, this, &ListViewItemWidget::moveFriendTo);
    }

    QAction *sendMsg = new QAction(QIcon(":/Resource/mainwidget/sendmsg.png"), "发送即时消息", this);
    QAction *showinfo = new QAction(QIcon(":/Resource/mainwidget/msgmgr.png"), "查看资料",this);
    QAction *updateremark = new QAction(QIcon(":/Resource/mainwidget/name.png"), "修改备注", this);
    QAction *del = new QAction(QIcon(":/Resource/mainwidget/deluser.png"), "删除好友", this);

    personMenu->addAction(sendMsg);
    personMenu->addAction(showinfo);
    personMenu->addAction(updateremark);
    personMenu->addMenu(movetoMenu);
    personMenu->addAction(del);

    connect(showinfo, &QAction::triggered, this, [this](){
        ShowInfoWidget *w = new ShowInfoWidget;

        w->setInfo(DataBase::getInstance()->getFriendInfo(userid));

        w->show();
    });

    connect(sendMsg, &QAction::triggered, this, &ListViewItemWidget::listWidgetMenuTriggered);

    connect(del, &QAction::triggered, this, [this](){
         DataBase::getInstance()->deleteFriend(userid);
         ConnectToServer::getInstance()->sendRequestDeleteFriend(userid);
         emit deleteFriend();
    });
}

void ListViewItemWidget::resizeEvent(QResizeEvent *event)
{
    m_headicon->move(5, (height() - 30) / 2);

    m_status->move(width() - 40, 5);

    m_nickname->resize(width() - 80, height() - 10);
    m_nickname->move(40, 5);

    QWidget::resizeEvent(event);
}

void ListViewItemWidget::setUserinfo(const QString &userid, const QString &username, const QString &signature)
{
    this->userid = userid;
    this->username = username;
    m_nickname->setText(QString("<em style=\"font-size:14px; color:#123;\">%1</em> <span style=\"font-size:12px; color:#123;\">%2</span>").arg(username, signature));
}

QString ListViewItemWidget::getUsername()
{
    return username;
}

QString ListViewItemWidget::getUserid()
{
    return userid;
}

void ListViewItemWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QWidget::contextMenuEvent(event);           //调用基类事件
    personMenu->exec(QCursor::pos());
}

void ListViewItemWidget::listWidgetMenuTriggered()
{
    QMap<QString, ChatWidget*>& chatwidgets = AllVariable::getChatWidgetMap();

    ChatWidget *chat = chatwidgets.value(userid);

    if (chat == NULL)
    {
        chat = new ChatWidget();
        chatwidgets.insert(userid, chat);
        chat->setIcon(imagePath);
    }

    QSettings *setting = RWSetting::getInstance()->getSetting();
    QStringList us = setting->value("RecentlyChat").toStringList();

    for (QStringList::iterator iter = us.begin(); iter != us.end();)
    {
        if (*iter == userid)
            iter = us.erase(iter);
        else
            ++iter;
    }

    us << userid;

    setting->setValue("RecentlyChat", us);

    chat->setUserName(username);
    chat->setUserid(userid);
    chat->show();
}

void ListViewItemWidget::setImage(const QString& url)
{
    HttpConnect *http = new HttpConnect();
    http->loadFileFormUrl(url);

    connect(http, &HttpConnect::loadCompleted, this, [this, http](){
        imagePath = http->getFilePath();
        m_headicon->setPixmap(QPixmap(imagePath));
    });
}

void ListViewItemWidget::moveFriendTo()
{
    QAction *s = static_cast<QAction*>(sender());
    DataBase::getInstance()->moveFriendToGroup(userid, s->text());
    ConnectToServer::getInstance()->sendRequestMoveFriendToGroup(userid, s->text());
    emit updateListWidget();
}

void ListViewItemWidget::setStatus(int status)
{
    switch (status) {
    case 1:
        m_status->setPixmap(QPixmap(":/Resource/status/online.png").scaled(32,32, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        break;
    case 2:
        m_status->setPixmap(QPixmap(":/Resource/status/offline.png").scaled(32,32, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        break;
    default:
        break;
    }
}


void ListViewItemWidget::updateGroupMenu()
{
    movetoMenu->clear();
    for(QString group : DataBase::getInstance()->getGroup())
    {
        QAction *action = new QAction(group, this);
        movetoMenu->addAction(action);
        connect(action, &QAction::triggered, this, &ListViewItemWidget::moveFriendTo);
    }
}
