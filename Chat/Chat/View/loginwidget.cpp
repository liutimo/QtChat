#include "loginwidget.h"
#include "mainwidget.h"
#include "chatwidget.h"
#include "BasicControls/headicon.h"
#include "BasicControls/loginstatusbar.h"
#include "Setting/rwsetting.h"
#include "DataBase/database.h"
#include "NetWork/connecttoserver.h"
#include "Thread/heartbeatthread.h"
#include "NetWork/msgstructure.h"

#include <QCryptographicHash>
#include <QEventLoop>
#include <QSettings>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QDebug>
#include <QTimerEvent>


LoginWidget::LoginWidget(QWidget *parent) : BasicWidget(parent),
    mainwidget(NULL)
{
    //设置大小不可变
    setAdjustmentSize(false);

    setFixedSize(320, 480);
    init();
    loadSetting();

    msg.status = 'a';
}

void LoginWidget::init()
{
    int w = width();

    hi_headicon = new HeadIcon(this);
    hi_headicon->setFixedSize(100, 100);
    hi_headicon->setPixmap(QPixmap(":/timg (1).jpg"));
    hi_headicon->move((w - hi_headicon->width()) / 2, 70);
    hi_headicon->setObjectName("hi_headicon");


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

    loginStatusBar = new LoginStatusBar(this);
    loginStatusBar->move(0, 480);
    loginStatusBar->resize(width(), 20);
    loginStatusBar->hide();


    connect(cb_rememberpw, &QCheckBox::stateChanged, this, &LoginWidget::addSetting);
    connect(cb_autologin, &QCheckBox::stateChanged, this, &LoginWidget::addSetting);
    connect(btn_login, &QPushButton::clicked, this, &LoginWidget::btn_login_clicked);
    connect(loginStatusBar, &LoginStatusBar::hide_status, this, &LoginWidget::hide_status);


    /*登陆状态*/
    connect(server, &ConnectToServer::connected, [](){qDebug() << "connected;";});
    connect(server, &ConnectToServer::loginStatus, this, &LoginWidget::loginStatus);
    connect(server, &ConnectToServer::responseHeartBeat, this, &LoginWidget::recvHeartBeat);
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
    strcpy(l->userid, cb_username->currentText().toUtf8().data());
    strcpy(l->password, QCryptographicHash::hash(le_password->text().toUtf8(), QCryptographicHash::Sha1).toHex());      //对密码进行哈希加密
    server->sendLoginMsg(l);

    delete l;
}

void LoginWidget::hide_status()
{
    hideStatusBar();
}

void LoginWidget::hideStatusBar()
{
    loginStatusBar->hide();
    setFixedHeight(this->height() - 20);
}

void LoginWidget::showStatusBar(const QString &text)
{
    loginStatusBar->show();
    loginStatusBar->setInfo(text);
    setFixedHeight(this->height() + 20);
}

void LoginWidget::loginStatus(LoginStatus ls)
{
    switch (ls)
    {
    case LOGINSUCCESS:
    {
        QThread::sleep(1);

        mainwidget = new MainWidget();
        connect(mainwidget, &MainWidget::loadFinished, this, &LoginWidget::showMainWidget);

        startTimer(2000);
        break;
    }
    case LOGINPWERROR:
        break;
    case LOGINUNKNOW:
    {
        showStatusBar("密码错误，请重新输入密码!");
        break;
    }
    case LOGINREPEAT:
    {
        QString str = QString("%1已经登陆,请勿重复登陆!").arg(cb_username->currentText());
        break;
    }
    default:
        break;
    }
}

void LoginWidget::recvHeartBeat()
{
    i = 0;
    qDebug() << "收到心跳包回复";
}

void LoginWidget::timerEvent(QTimerEvent *event)
{
    qDebug() << "发送心跳包";
    if(i > 3)
    {
        qDebug() << "离线";
        i = 0;
        killTimer(event->timerId());
    }
    else
    {
        i++;
        ConnectToServer::getInstance()->sendHeartBeatMsg(&msg);
    }
    event->accept();
}

void LoginWidget::socketError(QAbstractSocket::SocketError socketError)
{

}
void LoginWidget::showMainWidget()
{
    hide();
    mainwidget->show();
    qDebug() << "mainwidget load success";
}
