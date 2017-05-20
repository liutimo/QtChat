/**
*
*----------Dragon be here!----------/
* 　　　┏┓　　　┏┓
* 　　┏┛┻━━━┛┻┓
* 　　┃　　　　　　　┃
* 　　┃　　　━　　　┃
* 　　┃　┳┛　┗┳　┃
* 　　┃　　　　　　　┃
* 　　┃　　　┻　　　┃
* 　　┃　　　　　　　┃
* 　　┗━┓　　　┏━┛
* 　　　　┃　　　┃神兽保佑
* 　　　　┃　　　┃代码无BUG！
* 　　　　┃　　　┗━━━┓
* 　　　　┃　　　　　　　┣┓
* 　　　　┃　　　　　　　┏┛
* 　　　　┗┓┓┏━┳┓┏┛
* 　　　　　┃┫┫　┃┫┫
* 　　　　　┗┻┛　┗┻┛
* ━━━━━━神兽出没━━━━━━
*/


#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include "basicwidget.h"


class QPushButton;
class QLineEdit;
class QCheckBox;
class HeadIcon;
class QComboBox;

class LoginWidget : public BasicWidget
{
public:
    LoginWidget(QWidget *parent = Q_NULLPTR);


private:
    void init();

private:
    QPushButton *btn_login;
    QComboBox *cb_username;
    QLineEdit *le_password;
    HeadIcon *hi_headicon;
    QCheckBox *cb_rememberpw;
    QCheckBox *cb_autologin;
};

#endif // LOGINWIDGET_H
