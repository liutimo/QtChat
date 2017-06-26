#include "../allvariable.h"
#include "messagelistwidget.h"
#include "DataBase/database.h"
#include <BasicControls/messageitemwidget.h>
#include "View/chatwidget.h"
#include <QListWidget>
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

    if(s.size() == 0)
    {
        emit nonewmessage();
        this->hide();
    }

    for(auto elem : s)
    {
        QListWidgetItem *item = new QListWidgetItem();
        MessageItemWidget *w = new MessageItemWidget();
        w->setFriendName(DataBase::getInstance()->getFriendName(s.key(elem)));
        w->setNumber(elem->size());
        connect(w, &MessageItemWidget::itemclicked, this, &MessageListWidget::showChatWidget);
        listwidget->insertItem(listwidget->count(), item);
        listwidget->setItemWidget(item, w);
    }

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
}
