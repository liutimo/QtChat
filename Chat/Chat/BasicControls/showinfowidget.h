#ifndef SHOWINFOWIDGET_H
#define SHOWINFOWIDGET_H

#include "View/basicwidget.h"


QT_BEGIN_NAMESPACE
class HeadIcon;
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class ShowInfoWidget : public BasicWidget
{
public:
    ShowInfoWidget(QWidget* parent = Q_NULLPTR);

    void setInfo(const QVector<QString> &s);

private:

    void init();

    HeadIcon *headicon;

    QLabel *userid;
    QLabel *username;
    QLabel *mail;
    QLabel *mobile;
    QLabel *birthofdate;
    QLabel *personalizedsignature;
    QLabel *sex;

    QLabel *luserid;
    QLabel *lusername;
    QLabel *lmail;
    QLabel *lmobile;
    QLabel *lbirthofdate;
    QLabel *lpersonalizedsignature;
    QLabel *lsex;
};

#endif // SHOWINFOWIDGET_H
