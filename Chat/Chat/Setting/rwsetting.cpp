#include "rwsetting.h"

RWSetting::RWSetting(QObject *parent) : QObject(parent)
{
    configSettings = new QSettings("my.ini", QSettings::IniFormat);
}

RWSetting::~RWSetting()
{

}

QSettings *RWSetting::getSetting()
{
    return configSettings;
}

RWSetting* RWSetting::instance = NULL;
QMutex* RWSetting::mutex = new QMutex();

RWSetting* RWSetting::getInstance()
{

    mutex->lock();

    if(instance == NULL)
    {
        instance = new RWSetting();
    }

    mutex->unlock();

    return instance;
}
