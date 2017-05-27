/*****************************************
*          登录成功后的主界面               *
*    created by lz  2017/5/16 18:01      *
*****************************************/
#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "basicwidget.h"

class QPushButton;
class HeadIcon;
class LineEdit;
class QToolButton;

class MainWidget : public BasicWidget
{
    Q_OBJECT
public:
    MainWidget(QWidget *parent = Q_NULLPTR);

protected:
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent*);

private:
    void init();

signals:
    void changeBackGround(QColor color);

private slots:
    void changePureColorSkin(QColor );
    void changeImageSkin(const QString &);
    void showSkinManageWidget();
    void changSelectedButton();
private:
    typedef enum _SkinType{
        PURECOLOR = 0,          //纯色
        LOCALIMAGE,             //本地图片
        NETWORKIMAGE            //网络图片
    }SkinType;

    //标题栏按钮
    QPushButton *btn_mini;
    QPushButton *btn_skin;

    //头像
    SkinType skinType;
    QColor color;               //纯色皮肤颜色
    QString skinPath;           //本地文件皮肤

    HeadIcon *headIcon;

    //搜索
    LineEdit *le_serach;

    //三个导航按钮
    QToolButton *tb_contact;    //好友列表
    QToolButton *tb_group;      //群
    QToolButton *tb_last;       //最近聊天
};

#endif // MAINWIDGET_H
