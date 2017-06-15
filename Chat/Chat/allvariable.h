#ifndef ALLVARIABLE_H
#define ALLVARIABLE_H

#include <QObject>

class AllVariable : public QObject
{
    Q_OBJECT
public:
    explicit AllVariable(QObject *parent = nullptr);
    static void setLoginUserName(const QString &username);
    static QString getLoginUserName();
    static QMap<QString, QVector<QStringList>*>& getMessageMap();
signals:

public slots:

private:
    static QString loginusername;
    static QMap<QString, QVector<QStringList>*> messageMap;
};

#endif // ALLVARIABLE_H
