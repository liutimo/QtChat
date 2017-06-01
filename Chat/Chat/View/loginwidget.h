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
class QTimer;
class LoginStatusBar;


class LoginWidget : public BasicWidget
{
    Q_OBJECT
public:
    LoginWidget(QWidget *parent = Q_NULLPTR);


protected:
    void timerEvent(QTimerEvent *event);


private:
    void init();
    void loadSetting();
    void hideStatusBar();
    void showStatusBar(const QString &text);
private slots:
    void addSetting(int status);
    void btn_login_clicked();
    void loginStatus(LoginStatus);
    void recvHeartBeat();

    void hide_status();

private:
    QPushButton *btn_login;
    QComboBox *cb_username;
    QLineEdit *le_password;
    HeadIcon *hi_headicon;
    QCheckBox *cb_rememberpw;
    QCheckBox *cb_autologin;

    ConnectToServer *server;

    QTimer *timer;
    int  i = 0;
    HeartBeatMsg msg;

    LoginStatusBar *loginStatusBar;
};

#endif // LOGINWIDGET_H
