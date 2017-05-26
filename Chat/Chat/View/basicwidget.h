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
*
* 基础的窗口类，去除了系统自带的标题栏以及边框。
*/

#ifndef BASICWIDGET_H
#define BASICWIDGET_H

#include <QWidget>

class PushButton;

class BasicWidget : public QWidget
{
    Q_OBJECT
public:
    // 8个方向
    enum Orientation{NONE, LEFT, LEFTTOP, TOP, RIGHTTOP, RIGHT, RIGHTBOTTOM, BOTTOM, LEFTBOTTOM};
    explicit BasicWidget(QWidget *parent = 0);
    ~BasicWidget();
    void setAdjustmentSize(bool);

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

    PushButton* btn_close;

signals:

public slots:

private:
    void setOrientation(QMouseEvent *e);
    void setSize(QMouseEvent *e);
    bool pressed;
    bool pressed2; //边缘位置
    QPoint point;
    bool adjustment; // 窗口大小是否可以调整
    Orientation orientation; //记录8个方向
    QRect rect; //记录原始窗口大小
    QSize m_size;


};

#endif // BASICWIDGET_H
