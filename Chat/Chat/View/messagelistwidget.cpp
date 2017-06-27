#include "../allvariable.h"
#include "messagelistwidget.h"
#include "DataBase/database.h"
#include <BasicControls/messageitemwidget.h>
#include "View/chatwidget.h"
#include <QListWidget>
#include <QDebug>
MessageListWidget::MessageListWidget(QWidget *parent) : BasicWidget(parent)
{
    listwidget = new QListWidget(this);
}

void MessageListWidget::resizeEvent(QResizeEvent *event)
{
    BasicWidget::resizeEvent(event);
    listwidget->move(0, 32);
    listwidget->resize(width(), height() - 32);
}

void MessageListWidget::updateMessage()
{
    listwidget->clear();

    QMap<QString, QVector<QStringList>*> s = AllVariable::getMessageMap();
    QMap<QString, int> &g  = AllVariable::getGroupOfflineMessage();

    if(s.size() == 0 && g.size() == 0)
    {
        emit nonewmessage();
        this->hide();
        return;
    }

    //好友消息
    for(auto elem : s)
    {
        QListWidgetItem *item = new QListWidgetItem();
        MessageItemWidget *w = new MessageItemWidget();
        qDebug() << "friend name " << DataBase::getInstance()->getFriendName(s.key(elem));
        w->setFriendName(DataBase::getInstance()->getFriendName(s.key(elem)));
        w->setNumber(elem->size());
        w->setId(s.key(elem));
        w->setIcon(DataBase::getInstance()->getFriendImage(s.key(elem)));
        connect(w, &MessageItemWidget::itemclicked, this, &MessageListWidget::showChatWidget);
        listwidget->insertItem(listwidget->count(), item);
        listwidget->setItemWidget(item, w);
    }

    //群组消息
    for(int elem : g)
    {
        QListWidgetItem *item = new QListWidgetItem();
        MessageItemWidget *w = new MessageItemWidget();
        w->setFriendName(DataBase::getInstance()->getGroupName(g.key(elem)));
        w->setNumber(elem);
        w->setId(g.key(elem));
        w->setIcon(DataBase::getInstance()->getGroupIcon(g.key(elem)));

        connect(w, &MessageItemWidget::itemclicked, this, &MessageListWidget::showGroupChatWidget);
        listwidget->insertItem(listwidget->count(), item);
        listwidget->setItemWidget(item, w);
    }

}

void MessageListWidget::updateGroupMessage()
{
    updateMessage();
}

int MessageListWidget::getHeight()
{
    return 50 + listwidget->count() * 32;
}

void MessageListWidget::showChatWidget(const QString userid)
{
    ChatWidget *w;
    QMap<QString, ChatWidget*> &wm = AllVariable::getChatWidgetMap();
    w = wm.value(userid);
    if(w == NULL)
    {
        w = new ChatWidget();
        w->setUserName(DataBase::getInstance()->getFriendName(userid));
        w->setIcon(DataBase::getInstance()->getFriendImage(userid));
        w->setUserid(userid);
        wm.insert(userid, w);
    }

    QMap<QString, QVector<QStringList>*> &msgs = AllVariable::getMessageMap();

    QVector<QStringList>* s = msgs.value(userid);

    for(QStringList elem : *s)
    {
        //fontfamily << fontsize << fontcolor << message;
        w->showMessage(elem.at(3), elem.at(2), elem.at(2), elem.at(1));
    }

    msgs.remove(userid);
    w->show();
    this->updateMessage();
    hide();
}

void MessageListWidget::showGroupChatWidget(const QString groupid)
{
    QMap<QString, GroupChatWidget*> &map = AllVariable::getGroupChatWidget();

    GroupChatWidget *w = map.value(groupid);

    if (w == NULL)
    {
        w = new GroupChatWidget();
        w->setGroupId(groupid);
        w->setGroupName(DataBase::getInstance()->getGroupName(groupid));
        w->setIcon(DataBase::getInstance()->getGroupIcon(groupid));
        w->initMemberList();
        map.insert(groupid, w);
    }

    QVector<QStringList> vec= DataBase::getInstance()->getGroupOfflineMessage(groupid);

    for (QStringList elem : vec)
    {
        w->showMessage(elem[0], elem[1], elem[4], elem[2], elem[3]);
    }

    AllVariable::getGroupOfflineMessage().remove(groupid);
    w->show();
    updateMessage();
    hide();
}
