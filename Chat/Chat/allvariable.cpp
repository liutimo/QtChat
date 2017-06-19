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

QString AllVariable::getLoginUserId()
{
    return loginuserid;
}

void AllVariable::setLoginUserId(const QString&userid)
{
   loginuserid = userid;
}
