#include "DataBase/database.h"
#include "groupitemwidget.h"
#include "groupmemberlistwidget.h"
#include "groupmemberlineedit.h"
#include "listviewitemwidget.h"
#include "listwidget.h"
#include <QPushButton>
#include <QPainter>
#include <QDebug>
GroupMemberListWidget::GroupMemberListWidget(QWidget *parent) :
    QWidget(parent), isShow(true),
    isSelect(false)
{
    lineedit = new GroupMemberLineEdit(this);

    listwidget = new ListWidget(this);
    listwidget->setStyleSheet("background-color:rgba(255,255,255,200);");

    connect(listwidget, &ListWidget::doubleClicked, this, &GroupMemberListWidget::selectOneFriend);
}

void GroupMemberListWidget::paintEvent(QPaintEvent *event)
{
//    QPainter p(this);
//    p.setPen(QColor("#999"));
//    p.drawLine(0,0, 0, height());
}

void GroupMemberListWidget::resizeEvent(QResizeEvent *event)
{
    if (isShow)
    {
        lineedit->move(0, 0);
        lineedit->resize(width(), 30);

        listwidget->move(0, 30);

        listwidget->resize(width(), height() - 30);
    }
    else
    {
        lineedit->hide();
        listwidget->resize(width(), height());
    }
}

void GroupMemberListWidget::setList(const QVector<QStringList> &vec)
{
    listwidget->clear();
    for(int i = 0; i < vec.size(); ++i)
    {
        QStringList str = vec.at(i);
        GroupItemWidget *item_widget = new GroupItemWidget;
        item_widget->setImage(str.at(2));
        item_widget->setGroupInfo(str.at(0), str.at(1));
        item_widget->setStyleSheet("background-color:rgba(255,255,255,0);");
        item_widget->setGroup(false);
        item_widget->hideExit();

        QListWidgetItem *item = new QListWidgetItem;

        listwidget->insertItem(listwidget->count(), item);

        listwidget->setItemWidget(item, item_widget);
    }

}

void GroupMemberListWidget::addOne(const QString &userid, const QString &username, const QString &imagepath)
{
    GroupItemWidget *item_widget = new GroupItemWidget;
    item_widget->setImage(imagepath);
    item_widget->setGroupInfo(userid, username);
    item_widget->setStyleSheet("background-color:rgba(255,255,255,0);");
    QListWidgetItem *item = new QListWidgetItem;

    listwidget->insertItem(listwidget->count(), item);

    listwidget->setItemWidget(item, item_widget);
}

void GroupMemberListWidget::selectOneFriend(const QModelIndex &index)
{

    QListWidgetItem *current_item = listwidget->item(index.row());
    GroupItemWidget *w = static_cast<GroupItemWidget *>(listwidget->itemWidget(current_item));

    if(w != NULL && isSelect)
    {
        emit selectedOneFriend(w->getGroupId(), w->getGroupName(), w->getImagePath());

        listwidget->removeItemWidget(current_item);
        delete current_item;
    }
}

void GroupMemberListWidget::showSearchWidget(bool showflag)
{
    isShow = showflag;
}

void GroupMemberListWidget::setSelectWidget(bool selectflag)
{
    isSelect = selectflag;
}
