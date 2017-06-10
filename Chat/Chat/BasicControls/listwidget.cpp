#include "listwidget.h"
#include "listviewitemwidget.h"
#include "DataBase/database.h"
#include <QAction>
#include <QIcon>
#include <QDebug>

ListWidget::ListWidget(QWidget *parent) :
    QListWidget(parent)
{
    setFocusPolicy(Qt::NoFocus);       // 去除item选中时的虚线边框
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//水平滚动条关闭
    initMenu();



   qDebug() << "set friend list...........";
}
//初始化菜单
void ListWidget::initMenu()
{
    //初始化：
    blankMenu = new QMenu();
    groupMenu = new QMenu();
    personMenu = new QMenu();
    movetoMenu = new QMenu("移动好友至", this);
    movetoMenu->setIcon(QIcon(":/Resource/mainwidget/moveuser.png"));
    groupNameEdit=new QLineEdit();

    QAction *addGroup = new QAction("添加分组", this);
    QAction *delGroup = new QAction("删除该组", this);
    QAction *rename = new QAction("重命名", this);


    QAction *sendMsg = new QAction(QIcon(":/Resource/mainwidget/sendmsg.png"), "发送即时消息", this);
    QAction *showinfo = new QAction(QIcon(":/Resource/mainwidget/msgmgr.png"), "查看资料",this);
    QAction *updateremark = new QAction(QIcon(":/Resource/mainwidget/name.png"), "修改备注", this);
   // QAction *moveto = new QAction();
    QAction *del = new QAction(QIcon(":/Resource/mainwidget/deluser.png"), "删除好友", this);

    for(QString group : DataBase::getInstance()->getGroup())
    {
        QAction *action = new QAction(group, this);
        movetoMenu->addAction(action);
    }

    //设置：
    groupNameEdit->setParent(this);  //设置父类
    groupNameEdit->hide(); //设置初始时隐藏
    groupNameEdit->setPlaceholderText("未命名");//设置初始时的内容
    //布局：
    blankMenu->addAction(addGroup);
    groupMenu->addAction(delGroup);
    groupMenu->addAction(rename);

    personMenu->addAction(sendMsg);
    personMenu->addAction(showinfo);
    personMenu->addAction(updateremark);
    personMenu->addMenu(movetoMenu);
    personMenu->addAction(del);
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
    if(event->button()==Qt::LeftButton && currentItem!=NULL && currentItem==groupMap.value(currentItem))//如果点击的左键并且是点击的是组
    {
        if(isHideMap.value(currentItem))                                  //如果先前是隐藏，则显示
        {
            foreach(QListWidgetItem* subItem, groupMap.keys(currentItem))//遍历组的对应的项（包括自身和好友）
                if(subItem!=currentItem)                                 //如果是组的话不进行处理
                {
                    subItem->setHidden(false);                            //好友全部显示
                }
            isHideMap.insert(currentItem,false);                          //设置该组为显示状态
            currentItem->setIcon(QIcon(":/Resource/mainwidget/arrowdow.png"));
        }
        else                                                             //否则，先前是显示，则隐藏
        {
            foreach(QListWidgetItem* subItem, groupMap.keys(currentItem))//遍历组的对应的项（包括自身和好友）
                if(subItem!=currentItem)                                 //如果是组的话不进行处理
                {
                    subItem->setHidden(true);                            //好友全部隐藏
                }
             isHideMap.insert(currentItem,true);                          //设置该组为隐藏状态
             currentItem->setIcon(QIcon(":/Resource/mainwidget/arrowright"));
        }
    }
}
//菜单事件，为了显示菜单，点击鼠标右键响应，鼠标点击事件mousePressEvent优先于contextMenuEvent
void ListWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QListWidget::contextMenuEvent(event);           //调用基类事件
    if(currentItem==NULL)                           //如果点击到的是空白处
    {
        blankMenu->exec(QCursor::pos());
        return;
    }
    if(currentItem==groupMap.value(currentItem))    // 如果点击到的是组
        groupMenu->exec(QCursor::pos());
    else                                            //否则点击到的是好友
        personMenu->exec(QCursor::pos());
}

void ListWidget::setList(QList<QVector<QString>> friends, QStringList groups)
{
    for(int i = 0; i < groups.size(); ++i)
    {
        QString group = groups.at(i);

        QListWidgetItem *newItem=new QListWidgetItem(QIcon(":/Resource/mainwidget/arrowright.png"),group);
        newItem->setSizeHint(QSize(this->width(),25));
        this->addItem(newItem);

        groupMap.insert(newItem,newItem);
        isHideMap.insert(newItem,true);

        groupItemIndexMap.insert(group, i);
    }

    for(QVector<QString> onefriend : friends)
    {
        ListViewItemWidget *frienditem=new ListViewItemWidget();
        frienditem->setUserinfo(onefriend.at(0), onefriend.at(3));

        QListWidgetItem *newItem = new QListWidgetItem();
        this->insertItem(groupItemIndexMap.value(onefriend.at(2)) + 1,newItem);
        this->setItemWidget(newItem, frienditem);
        groupMap.insert(newItem,currentItem);   //加进容器，key为好友，value为组


        QMap<QString, int>::const_iterator iter = groupItemIndexMap.find(onefriend.at(2));
        for(; iter != groupItemIndexMap.cend(); ++iter)
        {
            groupItemIndexMap[iter.key()] = iter.value() + 1;
        }
    }
}
