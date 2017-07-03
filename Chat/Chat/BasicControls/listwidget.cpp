#include "listwidget.h"
#include "listviewitemwidget.h"
#include "recentchatitemwidget.h"
#include "groupitemwidget.h"
#include "DataBase/database.h"
#include "NetWork/connecttoserver.h"
#include <View/chatwidget.h>
#include <QAction>
#include <QIcon>
#include <QDebug>

ListWidget::ListWidget(QWidget *parent) :
    QListWidget(parent), showBlankMenu(false), renameGroup(false)
{
    setFocusPolicy(Qt::NoFocus);                                                     // 去除item选中时的虚线边框
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);                            //水平滚动条关闭
    initMenu();
}
//初始化菜单
void ListWidget::initMenu()
{

    blankMenu = new QMenu();
    groupMenu = new QMenu();

    groupNameEdit=new QLineEdit();

    QAction *addGroup = new QAction("添加分组", this);
    QAction *delGroup = new QAction("删除该组", this);
    QAction *rename = new QAction("重命名", this);

    groupNameEdit->setParent(this);
    groupNameEdit->hide();
    groupNameEdit->setPlaceholderText("未命名");

    blankMenu->addAction(addGroup);
    groupMenu->addAction(delGroup);
    groupMenu->addAction(rename);

    connect(addGroup, &QAction::triggered, this, [this](){
        QListWidgetItem *newItem=new QListWidgetItem(QIcon(":/Resource/mainwidget/arrowright.png"),"未命名");             //创建一个Item
        newItem->setSizeHint(QSize(this->width(),25));                                                                   //设置宽度、高度
        addItem(newItem);                                                                                                //加到QListWidget中
        groupNameEdit->raise();
        groupNameEdit->setText(tr("未命名"));                                                                             //设置默认内容
        groupNameEdit->selectAll();                                                                                      //设置全选
        groupNameEdit->setGeometry(this->visualItemRect(newItem).left() + 15,this->visualItemRect(newItem).top()+1,
                                   this->visualItemRect(newItem).width() - 15, this->visualItemRect(newItem).height()-2);//出现的位置
        groupNameEdit->show();                                                                                           //显示
        groupNameEdit->setFocus();                                                                                       //获取焦点
        currentItem = newItem;                                                                                           // 因为要给group命名，所以当前的currentItem设为该group

        vec.push_back(newItem);
    });

    connect(rename, &QAction::triggered, this, [this](){
        groupNameEdit->raise();
        groupNameEdit->setText(currentItem->text());
        groupNameEdit->selectAll();
        groupNameEdit->setGeometry(this->visualItemRect(currentItem).left() + 15,this->visualItemRect(currentItem).top()+1,
                                   this->visualItemRect(currentItem).width() - 15, this->visualItemRect(currentItem).height()-2); //出现的位置
        groupNameEdit->show();
        groupNameEdit->setFocus();

        renameGroup = true;
    });

    connect(delGroup, &QAction::triggered, this, [this](){
        ConnectToServer::getInstance()->sendDeleteGroupMsg(currentItem->text());
        DataBase::getInstance()->deleteFriendGroup(currentItem->text());
        removeItemWidget(currentItem);
        ishide.remove(currentItem->text());
        for(auto iter = vec.begin(); iter != vec.end(); ++iter)
        {
            if(*iter == currentItem)
            {
                vec.erase(iter);
                break;
            }
        }
        emit updateFriendGroupMenu();
        delete currentItem;
    });
}
//鼠标点击事件
void ListWidget::mousePressEvent(QMouseEvent *event)
{
    QListWidget::mousePressEvent(event);

    if(renameGroup)
    {
        renameGroup = false;
        groupNameEdit->hide();

        if(ishide.value(groupNameEdit->text()) != 0)                        //
        {
            return;
        }

        ishide.insert(groupNameEdit->text(), ishide.value(currentItem->text()));
        ishide.remove(currentItem->text());

        listmap.insert(groupNameEdit->text(), listmap.value(currentItem->text()));
        listmap.remove(currentItem->text());
        DataBase::getInstance()->deleteFriendGroup(currentItem->text());
        ConnectToServer::getInstance()->sendRenameFriendGroupMsg(currentItem->text(), groupNameEdit->text());

        currentItem->setText(groupNameEdit->text());
    }
    else if(groupNameEdit->isVisible() && !(groupNameEdit->rect().contains(event->pos())))
    {
        QString groupname = groupNameEdit->text();

        //如果 当前的分组名存在， 并且不为空就创建分组
        if(!groupname.isEmpty() && ishide.value(groupname) == 0) {
            currentItem->setText(groupname);
            ishide.insert(groupname, 1);
            listmap.insert(groupname, new QVector<QListWidgetItem*>());

            DataBase::getInstance()->addFriendGroup(groupname);
            emit updateFriendGroupMenu();
            ConnectToServer::getInstance()->sendRequestCreateGroup(groupname);
        }
        else
        {
            removeItemWidget(currentItem);
            delete currentItem;
        }
        groupNameEdit->setText("");
        groupNameEdit->hide();
    }
    currentItem = this->itemAt(mapFromGlobal(QCursor::pos()));//鼠标位置的Item，不管右键左键都获取
    if(event->button()==Qt::LeftButton && currentItem!=NULL)
    {
        QVector<QListWidgetItem*> *v = listmap.value(currentItem->text());

        if(v==NULL)
            return;

        if(ishide.value(currentItem->text()) == 1) {
            ishide.insert(currentItem->text(), 2);
            for(int i = 0; i < v->size(); ++i)
            {
                v->at(i)->setHidden(false);
            }
            isHide[currentItem] = false;
            currentItem->setIcon(QIcon(":/Resource/mainwidget/arrowdow.png"));
        }
        else
        {
            ishide.insert(currentItem->text(), 1);
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

    if(currentItem == NULL && showBlankMenu)
    {
        blankMenu->exec(QCursor::pos());
        return;
    }
    else if(currentItem != NULL && vec.contains(currentItem))
    {
        groupMenu->exec(QCursor::pos());
        return;
    }


}

void ListWidget::setList(QList<QVector<QString>> friends, QStringList groups)
{
    clear();
    groupItemIndex.clear();
    listmap.clear();
    friendmap.clear();

    for(int i = 0; i < groups.size(); ++i)
    {
        QString group = groups.at(i);
        QListWidgetItem *newItem=new QListWidgetItem(group);
        vec.push_back(newItem);
        newItem->setSizeHint(QSize(this->width(),25));
        this->addItem(newItem);

        isHide.insert(newItem, true);

        int flag = ishide.value(group);
        if (flag== 0)
        {
            ishide.insert(group, 1);
            newItem->setIcon(QIcon(":/Resource/mainwidget/arrowright.png"));
        }
        else if (flag == 1)
        {
            newItem->setIcon(QIcon(":/Resource/mainwidget/arrowright.png"));
        }
        else
        {
            newItem->setIcon(QIcon(":/Resource/mainwidget/arrowdow.png"));
        }
        groupItemIndex.append(new QPair<QString, int>(group, i));
        listmap.insert(group, new QVector<QListWidgetItem*>());
    }

    for(QVector<QString> onefriend : friends)
    {
        ListViewItemWidget *frienditem=new ListViewItemWidget();
        frienditem->setUserinfo(onefriend.at(0), onefriend.at(1), onefriend.at(4));
        frienditem->setImage(onefriend.at(5));
        frienditem->setStatus(onefriend.at(6).toInt());
        friendmap.insert(onefriend.at(0), frienditem);
        frienditem->setFixedSize(300, 50);

        frienditem->updateGroupMenu();

        connect(this, &ListWidget::updateFriendGroupMenu, frienditem, [frienditem](){
           frienditem->updateGroupMenu();
        });

        connect(frienditem, &ListViewItemWidget::updateListWidget, this, [this](){
            QStringList groups = DataBase::getInstance()->getGroup();
            QList<QVector<QString>> friends = DataBase::getInstance()->getFriendList();
            setList(friends, groups);
        });

        QString groupname = onefriend.at(3);

        QListWidgetItem *newItem = new QListWidgetItem();
        newItem->setText("");
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
        newItem->setSizeHint(QSize(250,50));
        qDebug() << newItem->sizeHint();

        int flag = ishide.value(groupname);
        if(flag == 1)
        {
            newItem->setHidden(true);
        }
        else
            newItem->setHidden(false);

        listmap.value(groupname)->append(newItem);

        connect(frienditem, &ListViewItemWidget::deleteFriend, this, [this,newItem, frienditem](){
//            removeItemWidget(newItem);
//            delete newItem;
            QStringList groups = DataBase::getInstance()->getGroup();
            QList<QVector<QString>> friends = DataBase::getInstance()->getFriendList();
            setList(friends, groups);
        });

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

void ListWidget::updateFriendStatus(const QString &userid, int status)
{
    friendmap.value(userid)->setStatus(status);
}

void ListWidget::setShowBlankMenu(bool flag)
{
    showBlankMenu = flag;
}
