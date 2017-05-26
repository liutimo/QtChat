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
#include "NetWork/msgstructure.h"

class QPushButton;
class QLineEdit;
class QCheckBox;
class HeadIcon;
class QComboBox;
class ConnectToServer;


class LoginWidget : public BasicWidget
{
public:
    LoginWidget(QWidget *parent = Q_NULLPTR);


private:
    void init();
    void loadSetting();

private slots:
    void addSetting(int status);
    void btn_login_clicked();
    void loginStatus(LoginStatus);

private:
    QPushButton *btn_login;
    QComboBox *cb_username;
    QLineEdit *le_password;
    HeadIcon *hi_headicon;
    QCheckBox *cb_rememberpw;
    QCheckBox *cb_autologin;

    ConnectToServer *server;
};

#endif // LOGINWIDGET_H
