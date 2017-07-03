#ifndef GROUPMEMBERLISTWIDGET_H
#define GROUPMEMBERLISTWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class GroupMemberLineEdit;
class ListWidget;
QT_END_NAMESPACE

class GroupMemberListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GroupMemberListWidget(QWidget *parent = nullptr);
    void setList(const QVector<QStringList> &vec);
    void addOne(const QString &userid, const QString &username, const QString &iamgepath);
    void showSearchWidget(bool showflag = false);
    void setSelectWidget(bool selectflag = false);                          //true则设置成可创建群组界面中的listwidget
signals:
    void selectedOneFriend(const QString &userid, const QString &username, const QString &imagepath);
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void selectOneFriend(const QModelIndex &index);

public slots:

private:
    GroupMemberLineEdit *lineedit;
    ListWidget *listwidget;

    bool isShow;                                //判断是否显示搜索框
    bool isSelect;                              //判断是否是创建群组窗口中的定制listwidget
};

#endif // GROUPMEMBERLISTWIDGET_H
