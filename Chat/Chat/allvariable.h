#ifndef ALLVARIABLE_H
#define ALLVARIABLE_H

#include <QObject>

#include "View/chatwidget.h"

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
    static QMap<QString, ChatWidget*>& getChatWidgetMap();
signals:

public slots:

private:
    static QString loginusername;
    static QString loginuserid;
    static QMap<QString, QVector<QStringList>*> messageMap;
    static QMap<QString, ChatWidget*> chatwidgetMap;
};

#endif // ALLVARIABLE_H
