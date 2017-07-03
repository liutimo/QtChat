#include "creategroup.h"

#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

#include "DataBase/database.h"
#include "BasicControls/groupmemberlistwidget.h"

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

    listwidget_yes = new GroupMemberListWidget(this);
//    listwidget->setList();
    listwidget_yes->setFixedSize(150, 300);
    listwidget_yes->move(50, 130);
    listwidget_yes->setList(DataBase::getInstance()->get_all_friends());


    listwidget_no = new GroupMemberListWidget(this);
    listwidget_no->setFixedSize(150, 300);
    listwidget_no->move(220, 130);


    ok_button = new QPushButton("创建", this);
    ok_button->setFixedSize(80, 30);
    ok_button->move(45, 450);

    cancel_button = new QPushButton("取消", this);
    cancel_button->setFixedSize(80, 30);
    cancel_button->move(250, 450);
}


