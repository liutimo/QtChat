#ifndef ALLVARIABLE_H
#define ALLVARIABLE_H

#include <QObject>

#include "View/chatwidget.h"
#include "View/groupchatwidget.h"
class AllVariable : public QObject
{
    Q_OBJECT
public:
    explicit AllVariable(QObject *parent = nullptr);

    static void setLoginUserName(const QString &username);
    static QString getLoginUserName();

    static QString getLoginUserId();
    static void setLoginUserId(const QString &);

    static QMap<QString, QVector<QStringList>*>& getMessageMap();


    //群组未读消息
    static QMap<QString, int>& getGroupOfflineMessage();

    static QMap<QString, ChatWidget*>& getChatWidgetMap();
    static QMap<QString, GroupChatWidget*>& getGroupChatWidget();
signals:

public slots:

private:
    static QString loginusername;
    static QString loginuserid;
    static QMap<QString, QVector<QStringList>*> messageMap;
    static QMap<QString, ChatWidget*> chatwidgetMap;
    static QMap<QString, GroupChatWidget*> groupchawtidgetMap;
    static QMap<QString, int> groupofflinemessageMap;
};

#endif // ALLVARIABLE_H
