#include "allvariable.h"
#include <QStringList>
#include <QVector>
#include <QMap>
AllVariable::AllVariable(QObject *parent) : QObject(parent)
{

}

QString AllVariable::loginusername = NULL;
QMap<QString, QVector<QStringList>*> AllVariable::messageMap = QMap<QString, QVector<QStringList>*>();
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
