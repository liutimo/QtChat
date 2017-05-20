#include "loginwidget.h"
#include "BasicControls/headicon.h"

#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>


LoginWidget::LoginWidget(QWidget *parent) : BasicWidget(parent)
{
    //设置大小不可变
    setAdjustmentSize(false);
    setFixedSize(320, 480);

    init();

}

void LoginWidget::init()
{
    int w = width();
    int h = height();


    hi_headicon = new HeadIcon(this);
    hi_headicon->setFixedSize(120, 120);
    hi_headicon->setPixmap(QPixmap(":/timg (1).jpg"));
    hi_headicon->move((w - hi_headicon->width()) / 2, 70);


    QLineEdit *line = new QLineEdit;
    line->setPlaceholderText("用户账号");
    cb_username = new QComboBox(this);
    cb_username->setFixedSize(200, 30);
    cb_username->setLineEdit(line);
    cb_username->move((w - cb_username->width()) / 2, 200);


    le_password = new QLineEdit(this);
    le_password->setFixedSize(200, 30);
    le_password->setPlaceholderText("用户密码");
    le_password->move((w - le_password->width()) / 2, 240);


    cb_rememberpw = new QCheckBox("记住密码", this);
    cb_rememberpw->setFixedSize(70, 20);
    cb_rememberpw->move(le_password->x(), 280);

    cb_autologin = new QCheckBox("自动登录", this);
    cb_autologin->setFixedSize(70, 20);
    cb_autologin->move(le_password->x() + 200 - cb_autologin->width(), 280);


    btn_login = new QPushButton("登录", this);
    btn_login->setFixedSize(200, 30);
    btn_login->move((w - btn_login->width()) / 2, 310);


}
