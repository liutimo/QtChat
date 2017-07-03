#ifndef CREATEGROUP_H
#define CREATEGROUP_H

#include "basicwidget.h"

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


private:
    void init();

    QLineEdit *group_name;
    QLabel *label1;
    QLabel *label2;

    GroupMemberListWidget *listwidget_yes;
    GroupMemberListWidget *listwidget_no;


    QPushButton *ok_button;
    QPushButton *cancel_button;
};

#endif // CREATEGROUP_H
