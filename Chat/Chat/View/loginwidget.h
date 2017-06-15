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

#include <QSystemTrayIcon>
#include <QAbstractSocket>

QT_BEGIN_NAMESPACE
class QMenu;
class QTimer;
class HeadIcon;
class QLineEdit;
class QCheckBox;
class QComboBox;
class MainWidget;
class QPushButton;
class LoginStatusBar;
class ConnectToServer;

QT_END_NAMESPACE

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
    void socketError(QAbstractSocket::SocketError socketError);
    void showMainWidget();
    void handleMessage(ReceivedMessageMsg *msg);
protected slots:
    void iconIsActived(QSystemTrayIcon::ActivationReason);
private:
    QPushButton *btn_login;
    QComboBox *cb_username;
    QLineEdit *le_password;
    HeadIcon *hi_headicon;
    QCheckBox *cb_rememberpw;
    QCheckBox *cb_autologin;

    ConnectToServer *server;

    int  i = 0;
    HeartBeatMsg msg;

    LoginStatusBar *loginStatusBar;

    MainWidget *mainwidget;

    void init_traymenu();
    QMenu *tray_menu;
    QSystemTrayIcon *tray;
    QTimer *timer;
    void setTrayIcon();
    int flag = 0;

};

#endif // LOGINWIDGET_H
