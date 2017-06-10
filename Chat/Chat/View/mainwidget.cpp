#include "mainwidget.h"
#include "skinmanagewidget.h"
#include "Setting/rwsetting.h"
#include "BasicControls/headicon.h"
#include "BasicControls/lineedit.h"
#include "BasicControls/listwidget.h"
#include "DataBase/database.h"
#include "NetWork/connecttoserver.h"


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


    le_serach = new LineEdit(this);
    le_serach->move(0, 123);

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

    connect(btn_skin, &QPushButton::clicked, this, &MainWidget::showSkinManageWidget);
    connect(tb_contact, &QToolButton::clicked, this, &MainWidget::changSelectedButton);
    connect(tb_group, &QToolButton::clicked, this, &MainWidget::changSelectedButton);
    connect(tb_last, &QToolButton::clicked, this, &MainWidget::changSelectedButton);

    connect(ConnectToServer::getInstance(), &ConnectToServer::responseFriendList, this, &MainWidget::receiveFriendList);
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

    tb_contact->resize(width() / 3, 30);
    tb_contact->move(0, 153);

    tb_group->resize(width() / 3, 30);
    tb_group->move(width() / 3, 153);

    tb_last->resize(width() / 3, 30);
    tb_last->move(width() / 3 * 2, 153);

    stackwidget->resize(width(), height() - 213);
    stackwidget->move(0, 183);

    le_serach->resize(width(), 30);
}

void MainWidget::paintEvent(QPaintEvent*event)
{
    QPainter p(this);
    p.setPen(Qt::NoPen);

    switch (skinType) {
    case PURECOLOR:
    {
        p.setBrush(color);
        p.drawRect(0, 0, width(), height());
        //emit changeBackGround(color);
        break;
    }
    case LOCALIMAGE:
    {
        p.drawPixmap(0, 0, width(), height(),
                     QPixmap(skinPath).copy(0,0,width(), height()));
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
        stackwidget->setCurrentIndex(1);
    else
        stackwidget->setCurrentIndex(2);
}

void MainWidget::receiveFriendList(QByteArray bytearray)
{

    parseFriend(bytearray);

    QStringList groups = DataBase::getInstance()->getGroup();
    QList<QVector<QString>> friends = DataBase::getInstance()->getFriendList();

    static_cast<ListWidget*>(stackwidget->widget(0))->setList(friends, groups);
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

                    onefriend.append(friendid);
                    onefriend.append(username);
                    onefriend.append(remark);
                    onefriend.append(personalizedsignature);
                    onefriend.append(grouptype);

                    friends.append(onefriend);
                }
            }
        }
    }
    else
        qDebug() << error.errorString();

    DataBase::getInstance()->setFriendList(friends);
}
