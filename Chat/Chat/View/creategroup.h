#ifndef CREATEGROUP_H
#define CREATEGROUP_H

#include "basicwidget.h"
#include <QMap>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QLabel;
class QPushButton;
class GroupMemberListWidget;
QT_END_NAMESPACE

class CreateGroup : public BasicWidget
{
    Q_OBJECT
public:
    explicit CreateGroup(QWidget *parent = nullptr);

signals:

private slots:
    void selectedOneFriend_yes(const QString &userid, const QString &username, const QString &imagepath);
    void selectedOneFriend_no(const QString &userid, const QString &username, const QString &imagepath);

private:
    void init();

    QLineEdit *group_name;
    QLabel *label1;
    QLabel *label2;

    GroupMemberListWidget *listwidget_yes;
    GroupMemberListWidget *listwidget_no;

    QPushButton *ok_button;
    QPushButton *cancel_button;

    QMap<QString, int> groupnames;                          //保存当前已经添加的userid。存在则不继续添加到listwidget_yes
};

#endif // CREATEGROUP_H
