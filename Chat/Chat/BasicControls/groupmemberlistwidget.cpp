#include "DataBase/database.h"
#include "groupmemberlistwidget.h"
#include "groupmemberlineedit.h"
#include "listwidget.h"
#include <QPushButton>
#include <QPainter>

GroupMemberListWidget::GroupMemberListWidget(QWidget *parent) : QWidget(parent)
{
    lineedit = new GroupMemberLineEdit(this);
    lineedit->setStyleSheet("background-color:rgba(255,255,255,0);");

    listwidget = new ListWidget(this);
    listwidget->setStyleSheet("background-color:rgba(255,255,255,0);");



}

void GroupMemberListWidget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setPen(QColor("#999"));
    p.drawLine(0,0, 0, height());
}

void GroupMemberListWidget::resizeEvent(QResizeEvent *event)
{
    lineedit->move(0, 0);
    lineedit->resize(200, 30);

    listwidget->move(0, 30);
    listwidget->resize(200, height());
}
