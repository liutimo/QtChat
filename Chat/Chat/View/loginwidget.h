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
class MessageListWidget;

QT_END_NAMESPACE

class LoginWidget : public BasicWidget
{
    Q_OBJECT
public:
    LoginWidget(QWidget *parent = Q_NULLPTR);
    void restore();

protected:
    void timerEvent(QTimerEvent *event);
    void paintEvent(QPaintEvent *e);

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
    void handleGroupMessage(ForwordGroupMessage *msg);
protected slots:
    void iconIsActived(QSystemTrayIcon::ActivationReason);
    void showMessageBox();
private:


    QPushButton *btn_login;
    QLineEdit *le_username;
    QLineEdit *le_password;
    HeadIcon *hi_headicon;
    QCheckBox *cb_rememberpw;
//    QCheckBox *cb_autologin;
    QPushButton *register_btn;
    QPushButton *findpassword_btn;

    ConnectToServer *server;

    int  i = 0;
    HeartBeatMsg msg;

    LoginStatusBar *loginStatusBar;

    MainWidget *mainwidget;

    void init_traymenu();
    QMenu *tray_menu;
    QAction *action_newmessage;
    QSystemTrayIcon *tray;
    QTimer *timer;
    void setTrayIcon();
    int flag = 0;
    MessageListWidget *l;

    bool exit = false;
    bool islogin = false;
};

#endif // LOGINWIDGET_H
