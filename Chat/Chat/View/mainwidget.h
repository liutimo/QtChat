/*****************************************
*          登录成功后的主界面               *
*    created by lz  2017/5/16 18:01      *
*****************************************/
#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "basicwidget.h"

class MainWidget : public BasicWidget
{
public:
    MainWidget();

protected:
    void resizeEvent(QResizeEvent *);

private:
    void init();

private:
    QPushButton *btn_mini;
    QPushButton *btn_skin;
};

#endif // MAINWIDGET_H
