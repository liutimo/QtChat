#include "rwsetting.h"

RWSetting::RWSetting()
{
    configSettings = new QSettings("my.ini", QSettings::IniFormat);
}

RWSetting::~RWSetting()
{
    delete configSettings;
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
