#include "listwidget.h"
#include "listviewitemwidget.h"
#include "recentchatitemwidget.h"
#include "groupitemwidget.h"
#include "DataBase/database.h"
#include <View/chatwidget.h>
#include <QAction>
#include <QIcon>
#include <QDebug>

ListWidget::ListWidget(QWidget *parent) :
    QListWidget(parent)
{
    setFocusPolicy(Qt::NoFocus);       // 去除item选中时的虚线边框
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//水平滚动条关闭
    initMenu();
}
//初始化菜单
void ListWidget::initMenu()
{
    //初始化：
    blankMenu = new QMenu();
    groupMenu = new QMenu();

    groupNameEdit=new QLineEdit();

    QAction *addGroup = new QAction("添加分组", this);
    QAction *delGroup = new QAction("删除该组", this);
    QAction *rename = new QAction("重命名", this);
    //设置：
    groupNameEdit->setParent(this);  //设置父类
    groupNameEdit->hide(); //设置初始时隐藏
    groupNameEdit->setPlaceholderText("未命名");//设置初始时的内容
    //布局：
    blankMenu->addAction(addGroup);
    groupMenu->addAction(delGroup);
    groupMenu->addAction(rename);
}
//鼠标点击事件
void ListWidget::mousePressEvent(QMouseEvent *event)
{
    QListWidget::mousePressEvent(event);

    if(groupNameEdit->isVisible() && !(groupNameEdit->rect().contains(event->pos())))
    {
        if(groupNameEdit->text()!=NULL)
            currentItem->setText(groupNameEdit->text());
        groupNameEdit->setText("");
        groupNameEdit->hide();
    }
    currentItem = this->itemAt(mapFromGlobal(QCursor::pos()));//鼠标位置的Item，不管右键左键都获取
    if(event->button()==Qt::LeftButton && currentItem!=NULL)
    {
        QVector<QListWidgetItem*> *v = listmap.value(currentItem->text());

        if(v==NULL)
            return;

        if(isHide.value(currentItem)) {
            for(int i = 0; i < v->size(); ++i)
            {
                v->at(i)->setHidden(false);
            }
            isHide[currentItem] = false;
            currentItem->setIcon(QIcon(":/Resource/mainwidget/arrowdow.png"));
        }
        else
        {
            for(int i = 0; i < v->size(); ++i)
            {
                v->at(i)->setHidden(true);
            }
            isHide[currentItem] = true;
            currentItem->setIcon(QIcon(":/Resource/mainwidget/arrowright.png"));
        }
    }
}

void ListWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QListWidget::contextMenuEvent(event);

    if(currentItem == NULL)
    {
        blankMenu->exec(QCursor::pos());
        return;
    }
}

void ListWidget::setList(QList<QVector<QString>> friends, QStringList groups)
{
    clear();
    groupItemIndex.clear();
    listmap.clear();

    for(int i = 0; i < groups.size(); ++i)
    {
        QString group = groups.at(i);
        QListWidgetItem *newItem=new QListWidgetItem(group);
        newItem->setIcon(QIcon(":/Resource/mainwidget/arrowright.png"));
        newItem->setSizeHint(QSize(this->width(),25));
        this->addItem(newItem);

        isHide.insert(newItem, true);

        groupItemIndex.append(new QPair<QString, int>(group, i));
        listmap.insert(group, new QVector<QListWidgetItem*>());
    }

    for(QVector<QString> onefriend : friends)
    {
        ListViewItemWidget *frienditem=new ListViewItemWidget();
        frienditem->setUserinfo(onefriend.at(0), onefriend.at(1), onefriend.at(4));
        frienditem->setImage(onefriend.at(5));
        connect(frienditem, &ListViewItemWidget::updateListWidget, this, [this](){
            QStringList groups = DataBase::getInstance()->getGroup();
            QList<QVector<QString>> friends = DataBase::getInstance()->getFriendList();
            setList(friends, groups);
        });

        QString groupname = onefriend.at(3);

        QListWidgetItem *newItem = new QListWidgetItem();
        int index = 0;

        for(int i = 0; i < groupItemIndex.size(); ++i)
        {
            if(groupItemIndex.at(i)->first == groupname)
            {
                index = i;
                break;
            }
        }

        insertItem(groupItemIndex.at(index)->second + 1,newItem);
        setItemWidget(newItem, frienditem);
        newItem->setHidden(true);
        listmap.value(groupname)->append(newItem);

        for(int i = index; i < groupItemIndex.size(); ++i)
        {
            groupItemIndex[i]->second++;
        }
    }
}

void ListWidget::setList(QVector<QStringList> lists)
{
    clear();
    for(QStringList elem : lists)
    {
        if (elem.size() == 0)
            continue;
        RecentChatItemWidget *itemwidget = new RecentChatItemWidget;
        itemwidget->setUserinfo(elem.at(0), elem.at(1), elem.at(2));
        itemwidget->setImage(elem.at(3));
        itemwidget->setMessage(elem.at(4));
        QListWidgetItem *newItem = new QListWidgetItem();
        newItem->setSizeHint(QSize(this->width(),50));
        insertItem(count(),newItem);
        setItemWidget(newItem, itemwidget);

        connect(itemwidget, &RecentChatItemWidget::delOneItem, this, [this,newItem](){
            removeItemWidget(newItem);
            delete newItem;
        });
    }
}

void ListWidget::setGroupList(QVector<QStringList> lists)
{
    clear();
    for(QStringList elem : lists)
    {

        GroupItemWidget *itemwidget = new GroupItemWidget;
        itemwidget->setGroupInfo(elem.at(0), elem.at(1));
        itemwidget->setImage(elem.at(2));
        QListWidgetItem *newItem = new QListWidgetItem();
        newItem->setSizeHint(QSize(this->width(),50));
        insertItem(count(),newItem);
        setItemWidget(newItem, itemwidget);
    }
}

void ListWidget::setList(QVector<QStringList> lists, int)
{
    clear();
    for(QStringList elem : lists)
    {
        if (elem.size() == 0)
            continue;
        ListViewItemWidget *itemwidget = new ListViewItemWidget;
        itemwidget->setUserinfo(elem.at(0), elem.at(1), elem.at(2));
        itemwidget->setImage(elem.at(3));
        QListWidgetItem *newItem = new QListWidgetItem();
        newItem->setSizeHint(QSize(this->width(),30));
        insertItem(count(),newItem);
        setItemWidget(newItem, itemwidget);
    }
}

void ListWidget::listWidgetMenuTriggered()
{
}
