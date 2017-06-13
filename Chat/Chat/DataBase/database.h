#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QFile>
#include <QMutex>
#include <QVariantList>

class DataBase : public QObject
{
    Q_OBJECT
public:

    QPair<QString, QString> getLocalUserInfo();
    void setLoaclUserInfo(const QString& userid, const QString &password);

    void setFriendList(QList<QVector<QString>> friends);
    QList<QVector<QString>> getFriendList();


    QStringList getGroup();

    static DataBase* getInstance();

signals:

public slots:

private:
    QSqlDatabase database;
    DataBase(QObject *parent = 0);

    static DataBase *instance;
    static QMutex *mutex;


};

#endif // DATABASE_H
