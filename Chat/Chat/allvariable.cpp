#include "allvariable.h"
#include <QStringList>
#include <QVector>
#include <QMap>
AllVariable::AllVariable(QObject *parent) : QObject(parent)
{

}

QString AllVariable::loginusername = NULL;
QMap<QString, QVector<QStringList>*> AllVariable::messageMap = QMap<QString, QVector<QStringList>*>();
QMap<QString, ChatWidget*> AllVariable::chatwidgetMap = QMap<QString, ChatWidget*>();
QMap<QString, GroupChatWidget*> AllVariable::groupchawtidgetMap = QMap<QString, GroupChatWidget*>();
QMap<QString, int> AllVariable::groupofflinemessageMap = QMap<QString, int>();
QString AllVariable::loginuserid = NULL;


void AllVariable::setLoginUserName(const QString &username)
{
    loginusername = username;
}

QString AllVariable::getLoginUserName()
{
    return loginusername;
}

QMap<QString, QVector<QStringList>*>& AllVariable::getMessageMap()
{
    return messageMap;
}

QMap<QString, ChatWidget*>& AllVariable::getChatWidgetMap()
{
    return chatwidgetMap;
}

QMap<QString, GroupChatWidget*>& AllVariable::getGroupChatWidget()
{
    return groupchawtidgetMap;
}

QString AllVariable::getLoginUserId()
{
    return loginuserid;
}

QMap<QString, int>& AllVariable::getGroupOfflineMessage()
{
    return groupofflinemessageMap;
}

void AllVariable::setLoginUserId(const QString&userid)
{
   loginuserid = userid;
}
