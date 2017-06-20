#include "chatwidget.h"
#include "../allvariable.h"
#include "DataBase/database.h"
#include "View/skinmanagewidget.h"
#include "BasicControls/headicon.h"
#include "NetWork/connecttoserver.h"
#include "BasicControls/pushbutton.h"

#include "Setting/rwsetting.h"

#include <QLabel>
#include <QDebug>
#include <QPainter>
#include <QDateTime>
#include <QTextEdit>
#include <QSettings>
#include <QPushButton>
#include <QTextObject>

ChatWidget::ChatWidget(QWidget *parent) : BasicWidget(parent)
{
    widgetIcon->hide();
    widgetTitle->hide();
    btn_close->hide();

    init();

    resize(650, 500);
}

void ChatWidget::init()
{
    headIcon = new HeadIcon(this);
    headIcon->setFixedSize(35, 35);
    headIcon->setPixmap(QPixmap(":/timg (1).jpg"));

    lb_username = new QLabel(tr("Username"), this);

    btn_chat_settings = new QPushButton(this);
    btn_chat_settings->setFixedSize(28, 28);
    btn_chat_settings->setObjectName("btn_chat_settings");

    btn_chat_close = new QPushButton(this);
    btn_chat_close->setFixedSize(28, 28);
    btn_chat_close->setObjectName("btn_chat_close");
    connect(btn_chat_close, &QPushButton::clicked, this, &BasicWidget::close);

    btn_chat_min = new QPushButton(this);
    btn_chat_min->setFixedSize(28, 28);
    btn_chat_min->setObjectName("btn_chat_min");

    btn_chat_max = new QPushButton(this);
    btn_chat_max->setFixedSize(28, 28);
    btn_chat_max->setObjectName("btn_chat_max");

    textedit = new QTextEdit(this);
    textedit->setReadOnly(true);
    textedit->setStyleSheet("background-color: rgba(255, 255, 255, 200);");

    chatinput = new ChatInput(this);
    chatinput->setObjectName("chatinput");
    chatinput->setStyleSheet("background-color: rgba(255, 255, 255, 200);");
    connect(chatinput, &ChatInput::sendMsg, this, &ChatWidget::setMessage);


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
    connect(SkinManageWidget::getInstance(), &SkinManageWidget::updatePureColorSkin, this, &ChatWidget::changePureColorSkin);
    connect(SkinManageWidget::getInstance(), &SkinManageWidget::updateImageSkin, this, &ChatWidget::changeImageSkin);

}

void ChatWidget::resizeEvent(QResizeEvent *event)
{
    headIcon->move(10, 10);

    lb_username->move(50, 12);
    lb_username->resize(width() - 172, 30);

    btn_chat_close->move(width() - 28, 0);
    btn_chat_max->move(width() - 56, 0);
    btn_chat_min->move(width() - 84, 0);
    btn_chat_settings->move(width() - 112, 0);

    textedit->resize(width(), height() - 240);
    textedit->move(0, 45);


    chatinput->resize(width() , 200);
    chatinput->move(0, height() - 200);

}

void ChatWidget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setPen(Qt::NoPen);

    switch (skinType) {
    case PURECOLOR:
    {
        color.setAlpha(200);
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

void ChatWidget::setMessage(const QString &msg)
{

    if(msg.isEmpty())
        return;

    QStringList fontinfo =  chatinput->getFontInfo();

    char *buf = new char[sizeof(RequestForwordMessageMsg) + msg.toUtf8().size()];

    RequestForwordMessageMsg *rmsg = (RequestForwordMessageMsg*)buf;
    strcpy(rmsg->friendid, userid.toUtf8().data());
    strcpy(rmsg->font, fontinfo.at(0).toUtf8().data());
    strcpy(rmsg->size, fontinfo.at(1).toUtf8().data());
    strcpy(rmsg->color, fontinfo.at(2).toUtf8().data());

    rmsg->length = msg.toUtf8().size();
    memcpy(rmsg->message, msg.toUtf8().data(), rmsg->length);

    ConnectToServer::getInstance()->sendRequestForwordMessageMsg(rmsg);
    delete []buf;


    QString html = QString("<html><b style=\"color:green; font-size:16px;\">%1</b> <em style=\"color:gray; font-size:12px;\">%2</em>"
                           "<br/>%3"
                           "<br/></html>").arg(AllVariable::getLoginUserName(), QDateTime::currentDateTime().toString("h:m:s ap"), msg);

    DataBase::getInstance()->setChatLog(AllVariable::getLoginUserName(), lb_username->text(), html);
    textedit->append(html);
}

void ChatWidget::showMessage(const QString &msg, const QString &color, const QString &size, const QString &family)
{
    QString html = QString("<html><b style=\"color:red; font-size:16px;\">%1</b> <em style=\"color:gray; font-size:12px;\">%2</em>"
                           "<br/><span style=\"color:%3; font-size:%4px;font-family:%5;\">%6</span>"
                           "<br/></html>").arg(lb_username->text(), QDateTime::currentDateTime().toString("h:m:s ap"), color, size, family, msg);

    DataBase::getInstance()->setChatLog(lb_username->text(), AllVariable::getLoginUserName(), html);
    textedit->append(html);
}

void ChatWidget::setUserName(const QString &username)
{
    lb_username->setText(username);
}

void ChatWidget::setUserid(const QString &userid)
{
    this->userid = userid;
}

void ChatWidget::setIcon(const QString &path)
{
    headIcon->setPixmap(QPixmap(path));
}



void ChatWidget::changePureColorSkin(QColor _color)
{
    RWSetting::getInstance()->getSetting()->setValue("SkinType", PURECOLOR);
    RWSetting::getInstance()->getSetting()->setValue("SKINCOLOR", _color);
    skinType = PURECOLOR;
    color = _color;
    update();
}
void ChatWidget::changeImageSkin(const QString &path)
{
    RWSetting::getInstance()->getSetting()->setValue("SkinType", LOCALIMAGE);
    RWSetting::getInstance()->getSetting()->setValue("SKINIMAGE", path);
    skinType = LOCALIMAGE;
    skinPath = path;
    update();
}
