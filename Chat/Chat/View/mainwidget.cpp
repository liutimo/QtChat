#include "../allvariable.h"
#include "mainwidget.h"
#include "skinmanagewidget.h"
#include "Setting/rwsetting.h"
#include "BasicControls/headicon.h"
#include "BasicControls/lineedit.h"
#include "BasicControls/listwidget.h"
#include "BasicControls/userlineedit.h"
#include "DataBase/database.h"
#include "NetWork/connecttoserver.h"
#include "NetWork/httpconnect.h"

#include <QPushButton>
#include <QResizeEvent>
#include <QPainter>
#include <QTime>
#include <QDebug>
#include <QStyle>
#include <QToolButton>
#include <QStackedWidget>
#include <QMenu>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QFontMetrics>

MainWidget::MainWidget(QWidget *parent) : BasicWidget(parent),
    skinType(PURECOLOR),
    color(QColor(40,138,221))
{
    init();
    loadSetting();

    setAdjustmentSize(true);
    setAutoFillBackground(true);
    setMinimumWidth(300);
    resize(300, 600);

    setWidgetTitle("这是一个主窗口");

}

void MainWidget::init()
{
    btn_mini = new QPushButton(this);
    btn_mini->setToolTip("最小化");
    btn_mini->setFixedSize(28, 26);
    btn_mini->setObjectName("btn_mini");

    btn_skin = new QPushButton(this);
    btn_skin->setToolTip("更换皮肤");
    btn_skin->setFixedSize(28, 26);
    btn_skin->setObjectName("btn_skin");
    btn_skin->setMouseTracking(false);

    headIcon = new HeadIcon(this);
    headIcon->setPixmap(QPixmap(":/timg (1).jpg"));
    headIcon->setFixedSize(64, 64);
    headIcon->move(15, 40);
    headIcon->setObjectName("mainwidget_headicon");

    username = new QLabel(this);
    username->setAlignment(Qt::AlignVCenter);
    username->setText("username");
    username->setStyleSheet("color: white;");

    personsignal = new UserLineEdit(this);

    searachwidget = new ListWidget(this);
    searachwidget->move(0, 153);
    searachwidget->hide();


    le_serach = new LineEdit(this);
    le_serach->move(0, 123);
    connect(le_serach, &LineEdit::LineEditFocusIn, this, [this](){
        tb_contact->hide();
        tb_group->hide();
        tb_last->hide();
        stackwidget->hide();
        searachwidget->show();
    });
    connect(le_serach, &LineEdit::LineEditFocusOut, this, [this](){
        tb_contact->show();
        tb_group->show();
        tb_last->show();
        stackwidget->show();
        searachwidget->hide();
    });
    connect(le_serach, &LineEdit::searachResult, this, [this](const QVector<QStringList> &s){
        searachwidget->setList(s, 1);
    });


    tb_status = new QToolButton(this);
    tb_status->setStyleSheet("QToolButton{border:0px;background-color: rgba(255, 255, 255, 0);}"
                             "QToolButton:hover{border:0px;background-color: rgba(255, 255, 255, 255);}");
    tb_status->setIcon(QIcon(":/Resource/status/imonline@2x.png"));

    //主界面状态菜单
    menu = new QMenu;
    init_menu();
    connect(tb_status, &QToolButton::clicked, this, [this](){menu->exec(mapToGlobal(tb_status->geometry().bottomLeft()));});

    tb_contact = new QToolButton(this);
    tb_contact->setObjectName("tb_contact");
    tb_contact->setProperty("selected", true);

    tb_group = new QToolButton(this);
    tb_group->setObjectName("tb_group");

    tb_last = new QToolButton(this);
    tb_last->setObjectName("tb_last");


    stackwidget = new QStackedWidget(this);


    ListWidget *friendList = new ListWidget;
    ListWidget *groupList = new ListWidget;
    ListWidget *recentList = new ListWidget;

    stackwidget->addWidget(friendList);
    stackwidget->addWidget(groupList);
    stackwidget->addWidget(recentList);


    btn_main_menu = new QToolButton(this);
    btn_main_menu->move(5, height() - 28);
    btn_main_menu->setFixedSize(24, 24);
    btn_main_menu->setObjectName("btn_main_menu");


    //主界面菜单
    main_menu = new QMenu();
    main_menu->addAction(new QAction("设置"));
    main_menu->addAction(new QAction("查看资料"));
    main_menu->addAction(new QAction("修改资料"));
    main_menu->addAction(new QAction("注销登陆"));
    main_menu->addAction(new QAction("退出"));

    btn_main_menu->setMenu(main_menu);
    btn_main_menu->setPopupMode(QToolButton::InstantPopup);
    //隐藏掉菜单的指示箭头
    btn_main_menu->setStyleSheet("QToolButton::menu-indicator{image:none;}");

    btn_add_friend = new QToolButton(this);
    btn_add_friend->move(50, height() - 30);
    btn_add_friend->resize(70, 28);
    btn_add_friend->setStyleSheet("QToolButton{border:0px;border-image: url(':/Resource/mainwidget/main_search_bkg.png');}"
                                  "QToolButton:hover{border:0px;border-image: url(':/Resource/mainwidget/menu_btn2_highlight@2x.png'}");

    connect(btn_skin, &QPushButton::clicked, this, &MainWidget::showSkinManageWidget);
    connect(tb_contact, &QToolButton::clicked, this, &MainWidget::changSelectedButton);
    connect(tb_group, &QToolButton::clicked, this, &MainWidget::changSelectedButton);
    connect(tb_last, &QToolButton::clicked, this, &MainWidget::changSelectedButton);




    RequestUserInfoMsg r;
    ConnectToServer::getInstance()->sendRequestUserInfoMsg(&r);

    ConnectToServer::getInstance()->sendRequestOfflineMessage();

    connect(ConnectToServer::getInstance(), &ConnectToServer::responseUserInfo, this, &MainWidget::receiveUserInfo);
    connect(ConnectToServer::getInstance(), &ConnectToServer::responseFriendList, this, &MainWidget::receiveFriendList);
    connect(ConnectToServer::getInstance(), &ConnectToServer::receivedOfflineMessage, this, &MainWidget::receiveOfflineMessage);

    connect(ConnectToServer::getInstance(), &ConnectToServer::receivedGroupInfo, this, &MainWidget::receivedGroupInfo);
    connect(ConnectToServer::getInstance(), &ConnectToServer::receivedGroupMemberInfo, this, &MainWidget::receivedGroupMemberInfo);
}

void MainWidget::loadSetting()
{
    QSettings *setting = RWSetting::getInstance()->getSetting();
    SkinType skintype = (SkinType)setting->value("SkinType").toInt();

    switch (skintype) {
    case PURECOLOR:
        changePureColorSkin((setting->value("SKINCOLOR")).value<QColor>());
        break;
    case LOCALIMAGE:
        changeImageSkin(setting->value("SKINIMAGE").toString());
    default:
        break;
    }
}

void MainWidget::resizeEvent(QResizeEvent *event)
{
    BasicWidget::resizeEvent(event);
    btn_mini->move(width() - btn_mini->width() - 28, 0);
    btn_skin->move(width() - btn_skin->width() - 56, 0);

    searachwidget->resize(width(), height() - 183);

    tb_contact->resize(width() / 3, 30);
    tb_contact->move(0, 153);

    tb_group->resize(width() / 3, 30);
    tb_group->move(width() / 3, 153);

    tb_last->resize(width() / 3, 30);
    tb_last->move(width() / 3 * 2, 153);

    stackwidget->resize(width(), height() - 213);
    stackwidget->move(0, 183);

    le_serach->resize(width(), 30);

    username->move(88, 43);
    personsignal->move(88, 75);
}

void MainWidget::paintEvent(QPaintEvent*event)
{
    QPainter p(this);
    p.setPen(Qt::NoPen);

    switch (skinType) {
    case PURECOLOR:
    {
        p.setBrush(color);
        p.drawRect(0, 0, width(), height() - 30);
        QColor c(color);
        c.setAlpha(100);
        p.setBrush(c);
        p.drawRect(0,  height() - 30, width(),30);
        //emit changeBackGround(color);
        break;
    }
    case LOCALIMAGE:
    {
        p.drawPixmap(0, 0, width(), height(),
                     QPixmap(skinPath).copy(0,0,width(), height()));
        QColor c(255, 255, 255, 100);
        p.setBrush(c);
        p.drawRect(0,  height() - 30, width(),30);
        break;
    }
    default:
        break;
    }
}

void MainWidget::showSkinManageWidget()
{
    SkinManageWidget *skin = SkinManageWidget::getInstance();
    skin->show();
    skin->move(this->geometry().right() + 1, this->geometry().y());
    connect(skin, &SkinManageWidget::updatePureColorSkin, this, &MainWidget::changePureColorSkin);
    connect(skin, &SkinManageWidget::updateImageSkin, this, &MainWidget::changeImageSkin);
}

void MainWidget::changePureColorSkin(QColor _color)
{
    RWSetting::getInstance()->getSetting()->setValue("SkinType", PURECOLOR);
    RWSetting::getInstance()->getSetting()->setValue("SKINCOLOR", _color);
    skinType = PURECOLOR;
    color = _color;
    update();
}

void MainWidget::changeImageSkin(const QString &path)
{
    RWSetting::getInstance()->getSetting()->setValue("SkinType", LOCALIMAGE);
    RWSetting::getInstance()->getSetting()->setValue("SKINIMAGE", path);
    skinType = LOCALIMAGE;
    skinPath = path;
    update();
}

void MainWidget::changSelectedButton()
{
    QToolButton *tb_sender = static_cast<QToolButton*>(sender());

    tb_contact->setProperty("selected", false);
    tb_group->setProperty("selected", false);
    tb_last->setProperty("selected", false);

    tb_sender->setProperty("selected", "true");

    tb_contact->style()->polish(tb_contact);
    tb_group->style()->polish(tb_group);
    tb_last->style()->polish(tb_last);

    if(tb_sender == tb_contact)
        stackwidget->setCurrentIndex(0);
    else if(tb_sender == tb_group)
    {
        stackwidget->setCurrentIndex(1);
        if (!isSend)
            ConnectToServer::getInstance()->sendRequestGroupInfo();
    }
    else
    {
        stackwidget->setCurrentIndex(2);
        QSettings *setting = RWSetting::getInstance()->getSetting();
        QStringList us = setting->value("RecentlyChat").toStringList();

        QVector<QStringList> lists = DataBase::getInstance()->getRecentlyChatFriendInfo(us);

        ListWidget* listwidget = dynamic_cast<ListWidget*>(stackwidget->widget(2));
        listwidget->setList(lists);
    }
}

void MainWidget::receiveFriendList(const QByteArray& bytearray)
{

    parseFriend(bytearray);

    ListWidget* listwidget = dynamic_cast<ListWidget*>(stackwidget->widget(0));

    QStringList groups = DataBase::getInstance()->getGroup();
    QList<QVector<QString>> friends = DataBase::getInstance()->getFriendList();

    listwidget->setList(friends, groups);

    emit loadFinished();
}


void MainWidget::init_menu()
{
    state_online = new QAction(QIcon(":/Resource/status/imonline@2x.png"), "在线");
    //    state_busy = new QAction(QIcon(":/Resource/status/busy@2x.png"), "忙碌");
    state_hide = new QAction(QIcon(":/Resource/status/invisible@2x.png"), "隐身");
    //    state_away = new QAction(QIcon(":/Resource/status/away@2x.png"), "离开");
    state_offline = new QAction(QIcon(":/Resource/status/imoffline@2x.png"), "离线");
    //    state_notdisturb = new QAction(QIcon(":/Resource/status/mute@2x.png"), "请勿打扰");

    menu->addAction(state_online);
    //    menu->addAction(state_busy);
    menu->addAction(state_hide);
    //    menu->addAction(state_away);
    menu->addAction(state_offline);
    //    menu->addAction(state_notdisturb);

    connect(state_online, &QAction::triggered, this, &MainWidget::changeStatus);
    //    connect(state_busy, &QAction::triggered, this, &MainWidget::changeStatus);
    connect(state_hide, &QAction::triggered, this, &MainWidget::changeStatus);
    //    connect(state_away, &QAction::triggered, this, &MainWidget::changeStatus);
    connect(state_offline, &QAction::triggered, this, &MainWidget::changeStatus);
    //    connect(state_notdisturb, &QAction::triggered, this, &MainWidget::changeStatus);

}

void MainWidget::changeStatus()
{
    QAction *action = static_cast<QAction*>(sender());

    UserStatus u;

    if (action == state_online)
    {
        tb_status->setIcon(QIcon(":/Resource/status/imonline@2x.png"));
        u = UserOnLine;
    }
    //    else if (action == state_busy)
    //    {
    //        tb_status->setIcon(QIcon(":/Resource/status/busy@2x.png"));
    //    }
    else if (action == state_hide)
    {
        u = UserHide;
        tb_status->setIcon(QIcon(":/Resource/status/invisible@2x.png"));
    }
    //    else if (action == state_away)
    //    {
    //        tb_status->setIcon(QIcon(":/Resource/status/away@2x.png"));
    //    }
    else if (action == state_offline)
    {
        tb_status->setIcon(QIcon(":/Resource/status/imoffline@2x.png"));
        u = UserOffLine;
    }
    //    else if (action == state_notdisturb)
    //    {
    //        tb_status->setIcon(QIcon(":/Resource/status/mute@2x.png"));
    //    }

    ConnectToServer::getInstance()->sendRequestChangeStatus(u);
}

void MainWidget::receiveUserInfo(const QByteArray& bytearry)
{
    parseUserInfo(bytearry);
}

void MainWidget::parseFriend(const QByteArray& bytearray)
{
    QList<QVector<QString>> friends;

    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(bytearray, &error);

    if(!document.isNull())
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();

            //a is friends group by grouptype
            for(auto name : object.keys())
            {
                QJsonArray array = object.value(name).toArray();
                int size = array.size();
                for(int i = 0; i < size; ++i)
                {
                    QVector<QString> onefriend;
                    QString friendid = array.at(i).toObject().value("friendid").toString();
                    QString username = array.at(i).toObject().value("username").toString();
                    QString remark = array.at(i).toObject().value("remark").toString();
                    QString grouptype = array.at(i).toObject().value("grouptype").toString();
                    QString personalizedsignature = array.at(i).toObject().value("personalizedsignature").toString();
                    QString imagepath = array.at(i).toObject().value("imagepath").toString();
                    QString birthofdate = array.at(i).toObject().value("birthofdate").toString();
                    QString sex = array.at(i).toObject().value("sex").toString();
                    QString mobile = array.at(i).toObject().value("mobile").toString();
                    QString mail = array.at(i).toObject().value("mail").toString();

                    onefriend.append(friendid);
                    onefriend.append(username);
                    onefriend.append(remark);
                    onefriend.append(personalizedsignature);
                    onefriend.append(grouptype);
                    onefriend.append(imagepath);
                    onefriend.append(birthofdate);
                    onefriend.append(sex);
                    onefriend.append(mobile);
                    onefriend.append(mail);

                    friends.append(onefriend);
                }
            }
        }
    }
    else
        qDebug() << error.errorString();

    DataBase::getInstance()->setFriendList(friends);

}


void MainWidget::parseUserInfo(const QByteArray &bytearray)
{
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(bytearray, &error);

    if(!document.isNull())
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();
            qDebug() << object.value("username").toString();
            qDebug() << object.value("ps").toString();
            QString url_str = object.value("imagepath").toString();

            QFontMetrics fm(this->font());

            username->setText(object.value("username").toString());
            AllVariable::setLoginUserName(username->text());

            tb_status->move(88 + fm.boundingRect(username->text()).width(), 43);

            personsignal->setText(object.value("ps").toString());

            QUrl url = QUrl::fromUserInput(url_str);
            QFile f(url.fileName());

            if(f.open(QIODevice::ReadOnly))
                headIcon->setPixmap(QPixmap(url.fileName()));
            else
            {
                qDebug() << "download image";
                HttpConnect *http = new HttpConnect();
                http->loadFileFormUrl(url_str);
                connect(http, &HttpConnect::loadCompleted, this, [this, http](){
                    headIcon->setPixmap(QPixmap(http->getFilePath()));}
                );
            }
        }
    }
    else
        qDebug() << error.errorString();
}

void MainWidget::receiveOfflineMessage(const QByteArray &bytearray)
{
    if(bytearray.size() < 3)
        return ;

    QMap<QString, QVector<QStringList>*>& map = AllVariable::getMessageMap();

    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(bytearray, &error);

    if(!document.isNull())
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();

            //a is friends group by grouptype
            for(auto name : object.keys())
            {
                QJsonArray array = object.value(name).toArray();
                int size = array.size();
                QVector<QStringList>* msgs = map.value(name);
                if(msgs == NULL)
                {
                    msgs = new QVector<QStringList>();
                    map.insert(name, msgs);
                }
                for(int i = 0; i < size; ++i)
                {
                    QStringList msg;
                    QString content = array.at(i).toObject().value("content").toString();
                    QString fontfamily = array.at(i).toObject().value("fontfamliy").toString();
                    QString fontsize = array.at(i).toObject().value("fontsize").toString();
                    QString fontcolor = array.at(i).toObject().value("fontcolor").toString();

                    msg << fontfamily << fontsize << fontcolor << content;
                    msgs->append(msg);
                }
            }
        }
    }
    else
        qDebug() << error.errorString();

    emit updateMessageBox();

    ConnectToServer::getInstance()->sendRequestAckOfflineMsg();

}

void MainWidget::parseGroup(const QByteArray&json)
{
    QVector<QStringList> lists;

    qDebug() << json;
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(json, &error);

    if(!document.isNull())
    {
        if(document.isArray())
        {
            QJsonArray array = document.array();

            for(int i = 0; i < array.size(); ++i)
            {
                QStringList list;
                QJsonValue value = array.at(i);
                list << value.toArray().at(0).toString();
                list << value.toArray().at(1).toString();
                list << value.toArray().at(2).toString();
                lists.append(list);
            }
        }

    }
    else
        qDebug() << error.errorString();
    DataBase::getInstance()->setGroupInfo(lists);
}

void MainWidget::receivedGroupInfo(const QByteArray&json)
{
    isSend = true;
    parseGroup(json);

    QVector<QStringList> lists = DataBase::getInstance()->getGroupInfo();

    ListWidget* listwidget = dynamic_cast<ListWidget*>(stackwidget->widget(1));
    listwidget->setGroupList(lists);

    ConnectToServer::getInstance()->sendRequestGroupMemberInfo();
    //    emit loadFinished();
}

void MainWidget::receivedGroupMemberInfo(const QByteArray &json)
{
    QMap<QString, QVector<QStringList>> map;

    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(json, &error);

    if(!document.isNull())
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();

            for(auto key : object.keys())
            {
                QVector<QStringList> vec;
                QJsonArray array = object.value(key).toArray();

                for(int i = 0; i < array.size(); ++i)
                {
                    QStringList list;
                    QJsonValue tmp = array.at(i);
                    list << tmp.toObject().value("memberid").toString();
                    list << tmp.toObject().value("membername").toString();
                    list << tmp.toObject().value("memberimage").toString();
                    vec.append(list);
                    qDebug() << list;
                }

                map.insert(key, vec);
            }
        }

    }
    else
        qDebug() << error.errorString();

    DataBase::getInstance()->setGroupMemberInfo(map);
}

void MainWidget::setSatus(Status status)
{
    switch (status) {
    case Online:
        tb_status->setIcon(QIcon(":/Resource/status/imonline@2x.png"));
        break;
    case Offline:
        tb_status->setIcon(QIcon(":/Resource/status/imoffline@2x.png"));
        break;
    case Hide:
        tb_status->setIcon(QIcon(":/Resource/status/invisible@2x.png"));
        break;
    case Busy:
        tb_status->setIcon(QIcon(":/Resource/status/busy@2x.png"));
        break;
    case NotDisturb:
        tb_status->setIcon(QIcon(":/Resource/status/mute@2x.png"));
        break;
    case Away:
        tb_status->setIcon(QIcon(":/Resource/status/away@2x.png"));
        break;
    default:
        break;
    }
}

