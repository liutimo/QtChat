#include "DataBase/database.h"
#include "groupitemwidget.h"
#include "groupmemberlistwidget.h"
#include "groupmemberlineedit.h"
#include "listviewitemwidget.h"
#include "listwidget.h"
#include <QPushButton>
#include <QPainter>

GroupMemberListWidget::GroupMemberListWidget(QWidget *parent) : QWidget(parent)
{
    lineedit = new GroupMemberLineEdit(this);
    lineedit->setStyleSheet("background-color:rgba(255,255,255,200);");

    listwidget = new ListWidget(this);
    listwidget->setStyleSheet("background-color:rgba(255,255,255,200);");



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

void GroupMemberListWidget::setList(const QVector<QStringList> &vec)
{
    listwidget->clear();
    for(int i = 0; i < vec.size(); ++i)
    {
        QStringList str = vec.at(i);
        ListViewItemWidget *item_widget = new ListViewItemWidget;
        item_widget->setImage(str.at(2));
        item_widget->setUserinfo(str.at(0), str.at(1), "");
        item_widget->setStyleSheet("background-color:rgba(255,255,255,0);");

        QListWidgetItem *item = new QListWidgetItem;

        listwidget->insertItem(listwidget->count(), item);

        listwidget->setItemWidget(item, item_widget);
    }

}
