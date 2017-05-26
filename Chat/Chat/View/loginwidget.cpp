#include "loginwidget.h"
#include "mainwidget.h"
#include "BasicControls/headicon.h"
#include "Setting/rwsetting.h"
#include "DataBase/database.h"
#include "NetWork/connecttoserver.h"


#include <QCryptographicHash>
#include <QSettings>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QDebug>

#include "NetWork/msgstructure.h"

LoginWidget::LoginWidget(QWidget *parent) : BasicWidget(parent)
{
    //设置大小不可变
    setAdjustmentSize(false);
    setFixedSize(320, 480);

    init();
    loadSetting();
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
    le_password->setEchoMode(QLineEdit::Password);
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


    server = ConnectToServer::getInstance();

    connect(cb_rememberpw, &QCheckBox::stateChanged, this, &LoginWidget::addSetting);
    connect(cb_autologin, &QCheckBox::stateChanged, this, &LoginWidget::addSetting);
    connect(btn_login, &QPushButton::clicked, this, &LoginWidget::btn_login_clicked);
    connect(server, &ConnectToServer::connected, [](){qDebug() << "connected;";});
    connect(server, &ConnectToServer::loginStatus, this, loginStatus);
}

void LoginWidget::addSetting(int status)
{
    QCheckBox *check = static_cast<QCheckBox*>(sender());

    if(check == cb_rememberpw)
    {
        RWSetting::getInstance()->getSetting()->setValue("记住密码", cb_rememberpw->isChecked());
    }

    if(check == cb_autologin)
    {
        RWSetting::getInstance()->getSetting()->setValue("自动登录", cb_rememberpw->isChecked());
    }
}

void LoginWidget::loadSetting()
{
    cb_rememberpw->setChecked(RWSetting::getInstance()->getSetting()->value("记住密码").toBool());
    cb_autologin->setChecked(RWSetting::getInstance()->getSetting()->value("自动登录").toBool());

    if(cb_rememberpw->isChecked())
    {
        DataBase *d = DataBase::getInstance();
        QPair<QString, QString> p = d->getLocalUserInfo();

        cb_username->setCurrentText(p.first);
        le_password->setText(p.second);
    }
    else
    {
        DataBase *d = DataBase::getInstance();
        QPair<QString, QString> p = d->getLocalUserInfo();

        cb_username->setCurrentText(p.first);
        le_password->setText("");
    }

}

void LoginWidget::btn_login_clicked()
{
    //更新数据库。
    DataBase *d = DataBase::getInstance();
    d->setLoaclUserInfo(cb_username->currentText(), le_password->text());



    LoginMsg *l = new LoginMsg();

    strcpy(l->userid, "123456");
    //对密码进行哈希加密
    strcpy(l->password, QCryptographicHash::hash(le_password->text().toUtf8(), QCryptographicHash::Sha1).toHex());

    server->sendLoginMsg(*l);

    delete l;
}

void LoginWidget::loginStatus(LoginStatus ls)
{
    qDebug() << ls;

    switch (ls)
    {
    case LOGINSUCCESS:
    {
        MainWidget *w = new MainWidget(NULL);
        w->show();
        this->close();
    }
        break;
    case LOGINPWERROR:
    case LOGINUNKNOW:
        qDebug() << "登录错误";
        break;
    }
}
