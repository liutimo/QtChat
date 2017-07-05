#include "creategroup.h"

#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>
#include "DataBase/database.h"
#include "BasicControls/groupmemberlistwidget.h"
#include "NetWork/connecttoserver.h"

CreateGroup::CreateGroup(QWidget *parent) : BasicWidget(parent)
{
    setFixedSize(400, 500);
    init();

}

void CreateGroup::init()
{
    label1 = new QLabel("聊天群名称:", this);
    label1->move(25, 55);

    group_name = new QLineEdit(this);
    group_name->move(105, 50);
    group_name->resize(260, 30);


    label2 = new QLabel("添加群成员:", this);
    label2->move(25, 100);

    listwidget_no = new GroupMemberListWidget(this);
    //    listwidget->setList();
    listwidget_no->setFixedSize(150, 320);
    listwidget_no->move(50, 130);
    listwidget_no->setSelectWidget(true);
    listwidget_no->setList(DataBase::getInstance()->get_all_friends());
    connect(listwidget_no, &GroupMemberListWidget::selectedOneFriend, this, &CreateGroup::selectedOneFriend_yes);

    listwidget_yes = new GroupMemberListWidget(this);
    listwidget_yes->setFixedSize(150, 290);
    listwidget_yes->move(220, 160);
    listwidget_yes->showSearchWidget(false);
    listwidget_yes->setSelectWidget(true);
    connect(listwidget_yes, &GroupMemberListWidget::selectedOneFriend, this, &CreateGroup::selectedOneFriend_no);

    ok_button = new QPushButton("创建", this);
    ok_button->setFixedSize(80, 30);
    ok_button->move(45, 450);

    //将添加入群组的好友转化成json
    connect(ok_button, &QPushButton::clicked, this, [this](){
        QJsonDocument json;
        QJsonArray array;
        for(QString userid : groupnames.keys())
        {
           QJsonValue v(userid);
           array.insert(array.count(), v);
        }

        json.setArray(array);

        if(group_name->text().isEmpty())
            return;

        ConnectToServer::getInstance()->sendCreateChatGroupMsg(group_name->text(), json.toJson());
        ConnectToServer::getInstance()->sendRequestGroupInfo();
        close();
    });

    cancel_button = new QPushButton("取消", this);
    cancel_button->setFixedSize(80, 30);
    cancel_button->move(250, 450);
    connect(cancel_button, &QPushButton::clicked, this, [this](){close();});
}


void CreateGroup::selectedOneFriend_yes(const QString &userid, const QString &username, const QString &imagepath)
{
    if(groupnames.value(userid) == 0)
    {
        listwidget_yes->addOne(userid, username, imagepath);
        groupnames.insert(userid, 1);
    }
}

void CreateGroup::selectedOneFriend_no(const QString &userid, const QString &username, const QString &imagepath)
{
    listwidget_no->addOne(userid, username, imagepath);
    groupnames.remove(userid);
}
