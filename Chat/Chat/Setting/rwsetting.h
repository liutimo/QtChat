#ifndef RWSETTING_H
#define RWSETTING_H

#include <QSettings>
#include <QVariant>
#include <QMutex>

class RWSetting
{
public:

    ~RWSetting();

    static RWSetting *getInstance();

    QSettings *getSetting();

private:
    RWSetting();
    QSettings *configSettings;

    static QMutex *mutex;
    static RWSetting *instance;
};

#endif // RWSETTING_H
