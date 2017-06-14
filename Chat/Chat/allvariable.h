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
signals:

public slots:

private:
    static QString loginusername;
};

#endif // ALLVARIABLE_H
