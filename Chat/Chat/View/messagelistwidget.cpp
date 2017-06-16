#include "../allvariable.h"
#include "messagelistwidget.h"
#include <BasicControls/messageitemwidget.h>
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

    for(auto elem : s)
    {
        QListWidgetItem *item = new QListWidgetItem();
        MessageItemWidget *w = new MessageItemWidget();
        w->setFriendName(s.key(elem));
        w->setNumber(elem->size());

        listwidget->insertItem(listwidget->count(), item);
        listwidget->setItemWidget(item, w);
    }

}

int MessageListWidget::getHeight()
{
    return 50 + listwidget->count() * 32;
}
