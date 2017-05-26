#include "mainwidget.h"
#include <QPushButton>
#include <QResizeEvent>
MainWidget::MainWidget()
{
    init();

    setMinimumWidth(300);
    resize(300, 600);

}

void MainWidget::init()
{
    btn_mini = new QPushButton(this);
    btn_mini->setToolTip("最小化");
    btn_mini->setFixedSize(30, 30);
    btn_mini->setObjectName("btn_mini");

    btn_skin = new QPushButton(this);
    btn_skin->setToolTip("更换皮肤");
    btn_skin->setFixedSize(30, 30);
    btn_skin->setObjectName("btn_skin");
}

void MainWidget::resizeEvent(QResizeEvent *event)
{
    BasicWidget::resizeEvent(event);
    btn_mini->move(width() - btn_mini->width() - 30, 0);
    btn_skin->move(width() - btn_skin->width() - 60, 0);
}
