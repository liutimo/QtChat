#ifndef ListWidget_H
#define ListWidget_H
#include <QListWidget>
#include <QMenu>
#include <QMouseEvent>
#include <QLineEdit>
//自定义QListWidget
class ListWidget : public QListWidget //继承QListWidget，可以使用它本身自带的函数，更方便
{
    Q_OBJECT
public:
    explicit ListWidget(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *event);//鼠标点击事件
    void contextMenuEvent(QContextMenuEvent*);//菜单事件，为了显示菜单
    void setList(QList<QVector<QString>>, QStringList);
    void setList(QVector<QStringList> lists);
    void setList(QVector<QStringList> lists, int);

private slots:
    void listWidgetMenuTriggered();
private:
    void initMenu();//初始化菜单

    QMenu *blankMenu;//点击空白上的菜单
    QMenu *groupMenu;//点击组上的菜单
    QMenu *movetoMenu;

    QMap<QString, QVector<QListWidgetItem*>*> listmap;
    QMap<QListWidgetItem*, bool> isHide;
    QLineEdit *groupNameEdit;//组的名字，重命名的时候需要用到
    QListWidgetItem *currentItem;//当前的项

    //保存每个分组的最后一个item的index
    //QMap<QString, int> groupItemIndexMap;
    QVector<QPair<QString, int>*> groupItemIndex;

    QPoint item_point;  //menu show on this point
signals:

public slots:

};
#endif // ListWidget_H

