#include "mainwidget.h"
#include "chatwidget.h"
#include "loginwidget.h"
#include "../allvariable.h"
#include "groupchatwidget.h"
#include "messagelistwidget.h"
#include "Setting/rwsetting.h"
#include "DataBase/database.h"
#include "NetWork/msgstructure.h"
#include "BasicControls/headicon.h"
#include "Thread/heartbeatthread.h"
#include "NetWork/connecttoserver.h"
#include "BasicControls/loginstatusbar.h"


#include <QMenu>
#include <QDebug>
#include <QTimer>
#include <QPainter>
#include <QSettings>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QEventLoop>
#include <QCloseEvent>
#include <QPushButton>
#include <QTimerEvent>
#include <QApplication>
#include <QSystemTrayIcon>
#include <QCryptographicHash>
#include <QDesktopServices>

LoginWidget::LoginWidget(QWidget *parent) : BasicWidget(parent),
    mainwidget(NULL)
{
    //设置大小不可变
    setAdjustmentSize(false);

    setFixedSize(320, 480);
    init();
    init_traymenu();
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



    le_username = new QLineEdit(this);
    le_username->setFixedSize(200, 30);
    le_username->setPlaceholderText("用户账号");
    le_username->move((w - le_username->width()) / 2, 200);
    le_username->setStyleSheet("QLineEdit{border:0px; border-radius:3px; background:rgba(255, 255, 255, 200);}");

    connect(le_username, &QLineEdit::textChanged, this, [this](const QString &text){
        QSettings *setting = RWSetting::getInstance()->getSetting();
        QString imagepath = setting->value("imagepath" + text).toString();
        qDebug() << imagepath;
        if(imagepath.isEmpty())
        {
            hi_headicon->setPixmap(QPixmap(":/timg (1).jpg"));
            hi_headicon->update();
        }
        else
        {
            hi_headicon->setPixmap(QPixmap(imagepath));
            hi_headicon->update();
        }
    });

    le_password = new QLineEdit(this);
    le_password->setFixedSize(200, 30);
    le_password->setPlaceholderText("用户密码");
    le_password->setEchoMode(QLineEdit::Password);
    le_password->move((w - le_password->width()) / 2, 240);
    le_password->setStyleSheet("QLineEdit{border:0px; border-radius:3px; background:rgba(255, 255, 255, 200);}");


    cb_rememberpw = new QCheckBox("记住密码", this);
    cb_rememberpw->setFixedSize(80, 20);
    cb_rememberpw->move(le_password->x(), 280);
    cb_rememberpw->setStyleSheet("QCheckBox{color:#0070d8;}");




    btn_login = new QPushButton("登录", this);
    btn_login->setFixedSize(200, 30);
    btn_login->move((w - btn_login->width()) / 2, 310);
    btn_login->setStyleSheet("QPushButton{border:0px; border-image:url(':/Resource/loginwidget/button_login_normal.png');}"
                                "QPushButton:hover{border:0px; border-image:url(':/Resource/loginwidget/button_login_hover.png');}"
                                "QPushButton:pressed{border:0px; border-image:url(':/Resource/loginwidget/button_login_down.png');}");

    register_btn = new QPushButton(this);
    register_btn->move(le_password->x(), 350);
    register_btn->setFixedSize(51, 16);
    register_btn->setStyleSheet("QPushButton{border:0px;border-image:url(':/Resource/loginwidget/zhuce.png');}"
                                "QPushButton:hover{border:0px;border-image:url(':/Resource/loginwidget/zhuce_hover.png');}"
                                "QPushButton:pressed{border:0px;border-image:url(':/Resource/loginwidget/zhuce_press.png');}");

    connect(register_btn, &QPushButton::clicked, this, [this](){
       QDesktopServices::openUrl(QUrl("http://localhost:8080/Chat/register.jsp"));
    });

    findpassword_btn = new QPushButton(this);
    findpassword_btn->move(210, 350);
    findpassword_btn->setFixedSize(51, 16);
    findpassword_btn->setStyleSheet("QPushButton{border:0px;border-image:url(':/Resource/loginwidget/mima.png');}"
                                "QPushButton:hover{border:0px;border-image:url(':/Resource/loginwidget/mima_hover.png');}"
                                "QPushButton:pressed{border:0px;border-image:url(':/Resource/loginwidget/mima_press.png');}");

    connect(findpassword_btn, &QPushButton::clicked, this, [this](){
       QDesktopServices::openUrl(QUrl("http://www.baidu.com"));
    });



    loginStatusBar = new LoginStatusBar(this);
    loginStatusBar->move(0, 480);
    loginStatusBar->resize(width(), 20);
    loginStatusBar->hide();


    connect(cb_rememberpw, &QCheckBox::stateChanged, this, &LoginWidget::addSetting);
//    connect(cb_autologin, &QCheckBox::stateChanged, this, &LoginWidget::addSetting);
    connect(btn_login, &QPushButton::clicked, this, &LoginWidget::btn_login_clicked);
    connect(loginStatusBar, &LoginStatusBar::hide_status, this, &LoginWidget::hide_status);
}

void LoginWidget::addSetting(int status)
{
    QCheckBox *check = static_cast<QCheckBox*>(sender());

    if(check == cb_rememberpw)
    {
        RWSetting::getInstance()->getSetting()->setValue("记住密码", cb_rememberpw->isChecked());
    }

//    if(check == cb_autologin)
//    {
//        RWSetting::getInstance()->getSetting()->setValue("自动登录", cb_rememberpw->isChecked());
//    }
}

void LoginWidget::loadSetting()
{
    cb_rememberpw->setChecked(RWSetting::getInstance()->getSetting()->value("记住密码").toBool());
//    cb_autologin->setChecked(RWSetting::getInstance()->getSetting()->value("自动登录").toBool());

    if(cb_rememberpw->isChecked())
    {
        DataBase *d = DataBase::getInstance();
        QPair<QString, QString> p = d->getLocalUserInfo();

        le_username->setText(p.first);
        le_password->setText(p.second);
    }
    else
    {
        DataBase *d = DataBase::getInstance();
        QPair<QString, QString> p = d->getLocalUserInfo();

        le_username->setText(p.first);
        le_password->setText("");
    }
}

void LoginWidget::btn_login_clicked()
{

    server = ConnectToServer::getInstance();

    if(server->state() == QAbstractSocket::UnconnectedState)
    {
        showStatusBar("网络不可达");
        server->close();
        return;
    }
    /*登陆状态*/
    connect(server, &ConnectToServer::connected, [](){qDebug() << "connected;";});
    connect(server, &ConnectToServer::loginStatus, this, &LoginWidget::loginStatus);
    connect(server, &ConnectToServer::responseHeartBeat, this, &LoginWidget::recvHeartBeat);

    /*message hite*/
    connect(server, &ConnectToServer::receivedMessage, this, &LoginWidget::handleMessage);
    connect(server, &ConnectToServer::disconnected, this, [this](){
//        QApplication::quit();
        islogin = false;
        mainwidget->setSatus(Offline);
        server->close();
        ;});
    connect(server, &ConnectToServer::receivedGroupMessage, this, &LoginWidget::handleGroupMessage);
    connect(server, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
            this, &LoginWidget::socketError);

    //更新数据库。
    DataBase *d = DataBase::getInstance();
    d->setLoaclUserInfo(le_username->text(), le_password->text());

    LoginMsg *l = new LoginMsg();
    strcpy(l->userid, le_username->text().toUtf8().data());
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
    setFixedHeight(500);
}

void LoginWidget::loginStatus(LoginStatus ls)
{
    switch (ls)
    {
    case LOGINSUCCESS:
    {
        islogin = true;
        AllVariable::setLoginUserId(le_username->text());

        QThread::sleep(1);
        mainwidget = new MainWidget();
        mainwidget->setLoginWidget(this);
        connect(mainwidget, &MainWidget::loadFinished, this, &LoginWidget::showMainWidget);
        connect(mainwidget, &MainWidget::updateMessageBox, this, [this](){
            timer->start();
            action_newmessage->setEnabled(true);
            l->updateMessage();
            l->setFixedSize(200, l->getHeight());
            //            ConnectToServer::getInstance();
        });

        //20s 发送一次心跳包
        startTimer(20000);
        tray->setToolTip(AllVariable::getLoginUserId());
        break;
    }
    case LOGINPWERROR:
    {
        showStatusBar("密码错误，请重新输入密码!");
        break;
    }
    case LOGINUNKNOW:
    {
        showStatusBar("CHAT帐号不存在！");
        break;
    }
    case LOGINREPEAT:
    {
        showStatusBar(QString("%1已经登陆,请勿重复登陆!").arg(le_username->text()));
        break;
    }
    default:{
        showStatusBar("未知错误！");
        break;
    }
    }
}

void LoginWidget::recvHeartBeat()
{
    i = 0;
}

void LoginWidget::timerEvent(QTimerEvent *event)
{
    if(i == 3)
    {
        i = 0;
        mainwidget->setSatus(Offline);

        if(exit)
            QApplication::quit();

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
    switch (socketError) {
    qDebug() <<socketError;
    case QAbstractSocket::RemoteHostClosedError:
        i = 3;
        if(exit)
            QApplication::quit();
        break;

    default:
        break;
    }
}
void LoginWidget::showMainWidget()
{
    hide();
    mainwidget->show();

}
void LoginWidget::init_traymenu()
{
    tray = new QSystemTrayIcon(this);
    tray->setIcon(QIcon(":/timg (1).jpg"));

    tray_menu = new QMenu();

    timer = new QTimer(this);
    timer->setInterval(500);
    connect(timer, &QTimer::timeout, this, &LoginWidget::setTrayIcon);

    action_newmessage = new QAction("查看新消息");
    connect(action_newmessage, &QAction::triggered, this, &LoginWidget::showMessageBox);

//    QAction *state_online = new QAction(QIcon(":/Resource/status/imonline@2x.png"), "在线");
//    QAction *state_busy = new QAction(QIcon(":/Resource/status/away@2x.png"), "忙碌");
//    QAction *state_hide = new QAction(QIcon(":/Resource/status/invisible@2x.png"), "隐身");
//    QAction *state_away = new QAction(QIcon(":/Resource/status/away@2x.png"), "离开");
//    QAction *state_Qme = new QAction(QIcon(":/Resource/status/imoffline@2x.png"), "离线");
//    QAction *state_notdisturb = new QAction(QIcon(":/Resource/status/mute@2x.png"), "请勿打扰");
    QAction *action_show = new QAction("打开主窗口");
    QAction *action_exit = new QAction("退出");

    connect(action_exit, &QAction::triggered, this, [this](){
        if(islogin) {
            ConnectToServer::getInstance()->sendRequestExitMessage();
        }
        else
            QApplication::exit();
        exit = true;
        if(!ConnectToServer::getInstance()->isWritable())
            QApplication::exit();
    });

    connect(action_show, &QAction::triggered, this, [this](){
        if(mainwidget == NULL)
        {
            show();
        }
        else
            mainwidget->show();
    });

    action_newmessage->setEnabled(false);
    tray_menu->addAction(action_newmessage);
//    tray_menu->addSeparator();
//    tray_menu->addAction(state_online);
//    tray_menu->addAction(state_busy);
//    tray_menu->addAction(state_hide);
//    tray_menu->addAction(state_away);
//    tray_menu->addAction(state_Qme);
//    tray_menu->addAction(state_notdisturb);
    tray_menu->addSeparator();
    tray_menu->addAction(action_show);
    tray_menu->addSeparator();
    tray_menu->addAction(action_exit);

    tray->setContextMenu(tray_menu);
    tray->show();
    connect(tray, &QSystemTrayIcon::activated, this, &LoginWidget::iconIsActived);

    l = new MessageListWidget();

    connect(l, &MessageListWidget::nonewmessage, this, [this](){
        action_newmessage->setEnabled(false);
        timer->stop();
        tray->setIcon(QIcon(":/timg (1).jpg"));
    });

    l->hide();
}

void LoginWidget::handleMessage(ReceivedMessageMsg *msg)
{
    char *m = new char[msg->length + 1];
    strcpy(m, msg->message);
    m[msg->length] = '\0';

    QString friendid(msg->friendid);
    QString fontfamily(msg->font);
    QString fontsize(msg->size);
    QString fontcolor(msg->color);
    QString message(m);

    ChatWidget *w = AllVariable::getChatWidgetMap().value(friendid);
    if(w == NULL || w->isHidden())
    {

        timer->start();
        action_newmessage->setEnabled(true);

        QStringList messageinfo;
        messageinfo << fontfamily << fontsize << fontcolor << message;

        QMap<QString, QVector<QStringList>*> &messagemap = AllVariable::getMessageMap();

        if(messagemap.value(friendid) == NULL)
        {
            QVector<QStringList> *messages = new QVector<QStringList>();
            messages->append(messageinfo);
            messagemap.insert(friendid, messages);
        }
        else
        {
            messagemap.value(friendid)->append(messageinfo);
        }
        delete msg;
        l->updateMessage();
        l->setFixedSize(200, l->getHeight());
    }
    else
        w->showMessage(message, fontcolor, fontsize, fontfamily);

}

void LoginWidget::handleGroupMessage(ForwordGroupMessage *msg)
{
    QMap<QString, GroupChatWidget*> &groupchatwidegt = AllVariable::getGroupChatWidget();

    GroupChatWidget *w = groupchatwidegt.value(msg->groupid);


    char *message = new char[msg->length + 1];
    strcpy(message, msg->message);
    message[msg->length] = '\0';

    if ( w == NULL || w->isHidden() )
    {
        //开始闪烁
        timer->start();

        //激活菜单项
        action_newmessage->setEnabled(true);

        //消息存入未读消息数据库
        DataBase::getInstance()->setGroupOfflineMessage(msg->groupid, msg->userid, message, msg->font, msg->size, msg->color);

        QMap<QString, int>& groupmessagemap = AllVariable::getGroupOfflineMessage();

        if(groupmessagemap.value(msg->groupid) == NULL)
        {
            groupmessagemap.insert(msg->groupid, 1);
        }
        else
        {
            groupmessagemap.insert(msg->groupid,groupmessagemap.value(msg->groupid)+1);
        }
        delete msg;
        l->updateGroupMessage();
        l->setFixedSize(200, l->getHeight());
    }
    else
        w->showMessage(msg->userid, message, msg->color, msg->size, msg->font);

}

void LoginWidget::setTrayIcon()
{
    if(flag % 2 == 0)
        tray->setIcon(QIcon(":/timg (1).jpg"));
    else
        tray->setIcon(QIcon());
    ++flag;
}
void LoginWidget::iconIsActived(QSystemTrayIcon::ActivationReason e)
{

}

void LoginWidget::showMessageBox()
{
    l->show();
    l->move((QCursor::pos()));
}

void LoginWidget::paintEvent(QPaintEvent *e)
{
    BasicWidget::paintEvent(e);
    QPainter p(this);

    p.drawPixmap(0, 0, QPixmap(":/Resource/138-14091F95623-50.jpg").scaled(width(), 480, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void LoginWidget::restore()
{
    loginStatusBar->hide();
    this->setFixedSize(width(), height() - 20);
    ConnectToServer::getInstance()->close();
}
