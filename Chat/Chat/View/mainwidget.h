/*****************************************
*          登录成功后的主界面               *
*    created by lz  2017/5/16 18:01      *
*****************************************/
#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "basicwidget.h"

QT_BEGIN_NAMESPACE
class QPushButton;
class HeadIcon;
class LineEdit;
class QToolButton;
class QStackedWidget;
class UserLineEdit;
class QMenu;
class ListWidget;
QT_END_NAMESPACE


typedef enum _Status{
    Online = 0,
    Offline,
    Hide,
    Busy,
    NotDisturb,
    Away
}Status;

class MainWidget : public BasicWidget
{
    Q_OBJECT
public:
    MainWidget(QWidget *parent = Q_NULLPTR);
    void setSatus(Status status);

protected:
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent*);

private:
    void init();
    void loadSetting();
signals:
    void changeBackGround(QColor color);
    void loadFinished();
    void updateMessageBox();
private slots:
    void changePureColorSkin(QColor );
    void changeImageSkin(const QString &);
    void showSkinManageWidget();
    void changSelectedButton();

    void receiveFriendList(const QByteArray&);
    void receiveUserInfo(const QByteArray&);
    void receiveOfflineMessage(const QByteArray&);
    void receiveGroupOfflineMessage(const QByteArray &);
    void receivedGroupInfo(const QByteArray&);
    void receivedGroupMemberInfo(const QByteArray&);
    void changeStatus();
    void friendStatusChange(const QString &, int);
    void receivedFriendAddRequest(const QString &sendid, const QString &validate);
private:
    typedef enum _SkinType{
        PURECOLOR = 0,          //纯色
        LOCALIMAGE,             //本地图片
        NETWORKIMAGE            //网络图片
    }SkinType;


    void parseFriend(const QByteArray&);
    void parseUserInfo(const QByteArray&);
    void parseGroup(const QByteArray&);
    void init_menu();
    void init_main_menu();

    //标题栏按钮
    QPushButton *btn_mini;
    QPushButton *btn_skin;

    //头像
    SkinType skinType;
    QColor color;               //纯色皮肤颜色
    QString skinPath;           //本地文件皮肤

    HeadIcon *headIcon;
    QLabel *username;
    UserLineEdit *personsignal;

    //搜索
    LineEdit *le_serach;

    QToolButton *tb_status;
    //三个导航按钮
    QToolButton *tb_contact;    //好友列表
    QToolButton *tb_group;      //群
    QToolButton *tb_last;       //最近聊天

    QStackedWidget *stackwidget;

    ListWidget *searachwidget;

//    QToolButton *btn_add_friend;

    QToolButton *btn_main_menu;

    QMenu *menu;
    QAction *state_online;
    QAction *state_busy;
    QAction *state_hide;
    QAction *state_away;
    QAction *state_offline;
    QAction *state_notdisturb;


    QMenu *main_menu;

    bool isSend = false;

    //这是一个不合适的解决方案。应该是我服务器没写好。导致服务端发送数据后客户端收不到离线消息。所以只能设置一个定时器定时的发送离线消息请求
    QTimer *timer;
    bool receiced = false;                                      //是否受到离线用户消息
    bool receicedgroupmsg = false;                              //是否受到群组离线消息
    bool isreceived = false;                                    //是否收到群组信息
};

#endif // MAINWIDGET_H
