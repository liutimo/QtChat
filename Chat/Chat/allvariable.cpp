#include "allvariable.h"

AllVariable::AllVariable(QObject *parent) : QObject(parent)
{

}

QString AllVariable::loginusername = NULL;

void AllVariable::setLoginUserName(const QString &username)
{
    loginusername = username;
}

QString AllVariable::getLoginUserName()
{
    return loginusername;
}
