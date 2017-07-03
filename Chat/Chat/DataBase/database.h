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
    static DataBase* getInstance();

    QPair<QString, QString> getLocalUserInfo();
    void setLoaclUserInfo(const QString& userid, const QString &password);

    void setFriendList(QList<QVector<QString>> friends);
    QList<QVector<QString>> getFriendList();

    QStringList getGroup();

    void setChatLog(const QString &senderid, const QString &receiverid, const QString &content);

    void moveFriendToGroup(const QString &userid, const QString &group);

    QVector<QString> getFriendInfo(const QString &userid);

    QVector<QStringList>  getRecentlyChatFriendInfo(const QStringList &);

    QVector<QStringList> searachFriend(const QString &key);

    void setGroupInfo(const QVector<QStringList> &lists);
    QVector<QStringList> getGroupInfo();

    void setGroupMemberInfo(const QMap<QString, QVector<QStringList>> &map);
    QVector<QStringList> getGroupMemberInfo(const QString &groupid);

    QString getFriendName(const QString &friendid);
    QString getFriendImage(const QString &friendid);

    QString getGroupName(const QString &groupid);

    QString getGroupMemberName(const QString &groupid, const QString &memberid);


    void setGroupOfflineMessage(const QString& groupid, const QString& senderid,
                                const QString& content, const QString& fontfamliy,
                                const QString& fontsize, const QString& fontcolor);
    QVector<QStringList> getGroupOfflineMessage(const QString &groupid);

    QString getGroupIcon(const QString &groupid);

    void deleteFriend(const QString &friendid);

    void addFriendGroup(const QString &groupname);

    void deleteFriendGroup(const QString &groupname);

    //创建群的时候使用
    QVector<QStringList> get_all_friends();

signals:
    void setGroupMemberFinished();
public slots:

private:
    QSqlDatabase database;
    DataBase(QObject *parent = 0);

    static DataBase *instance;
    static QMutex *mutex;


};

#endif // DATABASE_H
