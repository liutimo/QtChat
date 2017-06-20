#include "listwidget.h"
#include "listviewitemwidget.h"
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
    QListWidget::mousePressEvent(event); // 如果不调用基类mousePressEvent，item被select会半天不响应,调用父类，让QSS起效，因为QSS基于父类QListWidget，子类就是子窗口，就是最上层窗口，是覆盖在父窗口上的，所以先于父窗口捕获消息
    //防止一种特殊情况：给新item命名、点击其他item或空白处时，指向新item的currentItem被赋予其他item
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
//菜单事件，为了显示菜单，点击鼠标右键响应，鼠标点击事件mousePressEvent优先于contextMenuEvent
void ListWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QListWidget::contextMenuEvent(event);           //调用基类事件

    if(currentItem == NULL)                           //如果点击到的是空白处
    {
        blankMenu->exec(QCursor::pos());
        return;
    }

}

void ListWidget::setList(QList<QVector<QString>> friends, QStringList groups)
{
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


        QString groupname = onefriend.at(3);

        QListWidgetItem *newItem = new QListWidgetItem();
        int index = 0;

        for(int i = 0; i < groupItemIndex.size(); ++i)
        {
            if(groupItemIndex.at(i)->first == groupname)
            {
                index = groupItemIndex.at(i)->second;
                break;
            }
        }

        insertItem(index + 1,newItem);
        setItemWidget(newItem, frienditem);
        newItem->setHidden(true);
        listmap.value(groupname)->append(newItem);


        for(int i = index; i <groupItemIndex.size(); ++i)
        {
            groupItemIndex[i]->second++;
        }

    }
    for(int i = 0; i <groupItemIndex.size(); ++i)
    {
        qDebug() <<groupItemIndex[i]->first << groupItemIndex[i]->second;
    }

}

void ListWidget::listWidgetMenuTriggered()
{
    //    ChatWidget *w = new ChatWidget();
    //    w->show();

}
