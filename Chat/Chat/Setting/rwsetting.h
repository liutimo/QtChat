#ifndef RWSETTING_H
#define RWSETTING_H

#include <QObject>
#include <QSettings>
#include <QVariant>
#include <QMutex>

class RWSetting : public QObject
{
    Q_OBJECT
public:

    ~RWSetting();

    static RWSetting *getInstance();

    QSettings *getSetting();

private:
    RWSetting(QObject *parent = Q_NULLPTR);
    QSettings *configSettings;

    static QMutex *mutex;
    static RWSetting *instance;
};

#endif // RWSETTING_H
