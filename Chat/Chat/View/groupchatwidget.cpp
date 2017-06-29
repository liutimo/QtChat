#include "groupchatwidget.h"
#include "../allvariable.h"
#include "DataBase/database.h"
#include "View/skinmanagewidget.h"
#include "BasicControls/headicon.h"
#include "NetWork/connecttoserver.h"
#include "NetWork/httpconnect.h"
#include "BasicControls/pushbutton.h"
#include "BasicControls/groupitemwidget.h"
#include "BasicControls/groupmemberlistwidget.h"
#include "Setting/rwsetting.h"

#include <QLabel>
#include <QDebug>
#include <QPainter>
#include <QDateTime>
#include <QTextEdit>
#include <QSettings>
#include <QPushButton>
#include <QTextObject>
#include <QListWidget>

GroupChatWidget::GroupChatWidget(QWidget *parent) : BasicWidget(parent)
{
    widgetIcon->hide();
    widgetTitle->hide();
    btn_close->hide();

    init();

    resize(650, 500);
    setMouseTracking(true);


}

void GroupChatWidget::init()
{
    headIcon = new HeadIcon(this);
    headIcon->setFixedSize(35, 35);
    headIcon->setPixmap(QPixmap(":/timg (1).jpg"));
    headIcon->setStyleSheet("border:0px;background-color: transparent;");

    lb_groupname = new QLabel(tr("Username"), this);
    lb_groupname->setStyleSheet("border:0px;background-color: transparent;");

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
//    textedit->setAttribute(Qt::WA_TransparentForMouseEvents);
    textedit->setReadOnly(true);
    textedit->setStyleSheet("border-top:1px solid #999;border-bottom:1px solid #999;background-color: transparent;");
    textedit->installEventFilter(this);

    chatinput = new ChatInput(this);
    chatinput->setObjectName("chatinput");
    chatinput->setStyleSheet("background-color:transparent");
    chatinput->installEventFilter(this);
    connect(chatinput, &ChatInput::sendMsg, this, &GroupChatWidget::setMessage);

    listwidget = new GroupMemberListWidget(this);
    listwidget->setStyleSheet("background-color: transparent;");

    btn_hide_list = new QPushButton(this);
    btn_hide_list->setObjectName("btn_hide_list");
    btn_hide_list->setFixedSize(13, 90);
    btn_hide_list->setProperty("selected", hide_list);
    connect(btn_hide_list, &QPushButton::clicked, this, [this](){
        hide_list = !hide_list;
        btn_hide_list->setProperty("selected", hide_list);
        btn_hide_list->style()->polish(btn_hide_list);
        if(!hide_list) {
            listwidget->show();
            textedit->resize(width() - 200, height() - 240);
            textedit->move(0, 45);

            listwidget->move(width() - 200,45);
            listwidget->resize(200, height() - 45);

            btn_hide_list->move(width() - 213, 45 + textedit->height() / 2);

            chatinput->resize(width() - 200 , 200);
            chatinput->move(0, height() - 200);
        }
        else
        {
            textedit->resize(width(), height() - 240);
            textedit->move(0, 45);

            listwidget->hide();;

            btn_hide_list->move(width() - 13, 45 + textedit->height() / 2);

            chatinput->resize(width(), 200);
            chatinput->move(0, height() - 200);
        }
    });
}

void GroupChatWidget::resizeEvent(QResizeEvent *event)
{
    BasicWidget::resizeEvent(event);

    headIcon->move(10, 10);

    lb_groupname->move(50, 12);
    lb_groupname->resize(width() - 172, 30);

    btn_chat_close->move(width() - 28, 0);
    btn_chat_max->move(width() - 56, 0);
    btn_chat_min->move(width() - 84, 0);
    btn_chat_settings->move(width() - 112, 0);

    if(!hide_list) {
        listwidget->show();
        textedit->resize(width() - 200, height() - 240);
        textedit->move(0, 45);

        listwidget->move(width() - 199,45);
        listwidget->resize(200, height() - 45);

        btn_hide_list->move(width() - 213, 45 + textedit->height() / 2);

        chatinput->resize(width() - 200 , 200);
        chatinput->move(0, height() - 200);
    }
    else
    {
        textedit->resize(width(), height() - 240);
        textedit->move(0, 45);

        listwidget->hide();;

        btn_hide_list->move(width() - 13, 45 + textedit->height() / 2);

        chatinput->resize(width(), 200);
        chatinput->move(0, height() - 200);
    }

}

void GroupChatWidget::paintEvent(QPaintEvent *event)
{
    BasicWidget::paintEvent(event);
    QPainter p(this);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(234,233,248));
    p.drawRect(0, 0, width(), height());

}

void GroupChatWidget::setMessage(const QString &msg)
{

    if(msg.isEmpty())
        return;

    QStringList fontinfo =  chatinput->getFontInfo();

    char *buf = new char[sizeof(ForwordGroupMessage) + msg.toUtf8().size()];

    ForwordGroupMessage *rmsg = (ForwordGroupMessage*)buf;
    strcpy(rmsg->userid, AllVariable::getLoginUserId().toUtf8().data());
    strcpy(rmsg->groupid, groupid.toUtf8().data());
    strcpy(rmsg->font, fontinfo.at(0).toUtf8().data());
    strcpy(rmsg->size, fontinfo.at(1).toUtf8().data());
    strcpy(rmsg->color, fontinfo.at(2).toUtf8().data());

    rmsg->length = msg.toUtf8().size();
    memcpy(rmsg->message, msg.toUtf8().data(), rmsg->length);

    ConnectToServer::getInstance()->sendForwordGroupMessage(rmsg);
    delete []buf;

    QString html = QString("<html><b style=\"color:green; font-size:16px;\">%1</b> <em style=\"color:gray; font-size:12px;\">%2</em>"
                           "<br/>%3"
                           "<br/></html>").arg(AllVariable::getLoginUserName(), QDateTime::currentDateTime().toString("h:m:s ap"), msg);

    //存入本地数据库
//    DataBase::getInstance()->setChatLog(AllVariable::getLoginUserId(), groupid, html);
    textedit->append(html);

    emit updateMessage();
}

void GroupChatWidget::showMessage(const QString &senderid, const QString &msg, const QString &color, const QString &size, const QString &family)
{
    QString html = QString("<html><b style=\"color:red; font-size:16px;\">%1</b> <em style=\"color:gray; font-size:12px;\">%2</em>"
                           "<br/><span style=\"color:%3; font-size:%4px;font-family:%5;\">%6</span>"
                           "<br/></html>").arg(DataBase::getInstance()->getGroupMemberName(groupid, senderid), QDateTime::currentDateTime().toString("h:m:s ap"), color, size, family, msg);

    DataBase::getInstance()->setChatLog(groupid, AllVariable::getLoginUserId(), html);
    textedit->append(html);
}

void GroupChatWidget::setGroupName(const QString &username)
{
    lb_groupname->setText(username);
}

void GroupChatWidget::setGroupId(const QString &groupid)
{
    this->groupid = groupid;
}

void GroupChatWidget::setIcon(const QString &path)
{
    if(QFile(QUrl(path).fileName()).exists())
        headIcon->setPixmap(QPixmap(QUrl(path).fileName()));
    else
    {
        HttpConnect *http = new HttpConnect();
        http->loadFileFormUrl(path);

        connect(http, &HttpConnect::loadCompleted, this, [this, http](){
//            qDebug() << "设置头像";
            headIcon->setPixmap(QPixmap(http->getFilePath()));
        });
    }
}

void GroupChatWidget::initMemberList()
{
    QVector<QStringList> vec = DataBase::getInstance()->getGroupMemberInfo(groupid);

    listwidget->setList(vec);
}
