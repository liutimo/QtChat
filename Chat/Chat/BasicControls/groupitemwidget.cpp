#include "groupitemwidget.h"

#include "DataBase/database.h"
#include "NetWork/httpconnect.h"
#include "Setting/rwsetting.h"
#include "NetWork/connecttoserver.h"
#include "showinfowidget.h"
#include "View/groupchatwidget.h"
#include "../allvariable.h"
#include "headicon.h"

#include <QPixmap>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QSettings>

GroupItemWidget::GroupItemWidget(QWidget *parent) : QWidget(parent)
{
    init();
    resize(200, 50);
}

void GroupItemWidget::init()
{
    m_headicon = new HeadIcon(this);
    m_headicon->resize(30, 30);

    m_nickname = new QLabel(this);
    m_nickname->setTextFormat(Qt::RichText);

    m_nickname->setText("<b>我是昵称</b> 我是个性签名啊！！！！");

    groupMenu = new QMenu();

    QAction *sendMsg = new QAction(QIcon(":/Resource/mainwidget/sendmsg.png"), "发送即时消息", this);
    connect(sendMsg, &QAction::triggered, this, &GroupItemWidget::listWidgetMenuTriggered);


    groupMenu->addAction(sendMsg);
}

void GroupItemWidget::resizeEvent(QResizeEvent *event)
{
    m_headicon->move(5, (height() - 30) / 2);

    m_nickname->resize(width() - 80, 20);
    m_nickname->move(40, (height() - 20) / 2);
}
void GroupItemWidget::setGroupInfo(const QString &groupid, const QString &groupname)
{
    this->groupname = groupname;
    this->groupid = groupid;
    m_nickname->setText(QString("<em style=\"font-size:14px; color:#123;\">%1</em> ").arg(groupname));
}

QString GroupItemWidget::getGroupName()
{
    return groupname;
}

QString GroupItemWidget::getGroupId()
{
    return groupid;
}

void GroupItemWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QWidget::contextMenuEvent(event);           //调用基类事件
    groupMenu->exec(QCursor::pos());
}

void GroupItemWidget::listWidgetMenuTriggered()
{
    QMap<QString, GroupChatWidget*>& groupchatwidgets = AllVariable::getGroupChatWidget();

    GroupChatWidget *chat = groupchatwidgets.value(groupid);

    if (chat == NULL)
    {
        chat = new GroupChatWidget();
        groupchatwidgets.insert(groupid, chat);
        chat->initMemberList();
    }

    QSettings *setting = RWSetting::getInstance()->getSetting();
    QStringList us = setting->value("RecentlyGroupChat").toStringList();

    for (QStringList::iterator iter = us.begin(); iter != us.end();)
    {
        if (*iter == groupid)
            iter = us.erase(iter);
        else
            ++iter;
    }

    us << groupid;

    setting->setValue("RecentlyGroupChat", us);

    chat->setGroupId(groupid);
    chat->setGroupName(groupname);
    chat->setIcon(imagePath);
    chat->initMemberList();
    chat->show();
}

void GroupItemWidget::setImage(const QString& url)
{
    HttpConnect *http = new HttpConnect();

    imagePath = QUrl(url).fileName();
    if(QFile(imagePath).exists())
    {
        m_headicon->setPixmap(QPixmap(QUrl(url).fileName()));
    }
    else
    {
        http->loadFileFormUrl(url);
        connect(http, &HttpConnect::loadCompleted, this, [this, http](){
            m_headicon->setPixmap(QPixmap(imagePath));
        });
    }

}

