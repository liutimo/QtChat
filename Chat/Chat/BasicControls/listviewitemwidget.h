#ifndef LISTVIEWITEMWIDGET_H
#define LISTVIEWITEMWIDGET_H

#include <QWidget>

#include "NetWork/msgstructure.h"

QT_BEGIN_NAMESPACE
class QLabel;
class HeadIcon;
class QMenu;
QT_END_NAMESPACE

class ListViewItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ListViewItemWidget(QWidget *parent = 0);
    void setUserinfo(const QString &userid, const QString &username, const QString &signature);
    void setImage(const QString& url);
    QString getUsername();
    QString getUserid();
    void setStatus(int status);
    void updateGroupMenu();
signals:
    void updateListWidget();
    void deleteFriend();
public slots:

private slots:
    void moveFriendTo();

protected:
    void resizeEvent(QResizeEvent *);
    void contextMenuEvent(QContextMenuEvent *event);
    void listWidgetMenuTriggered();

private:
    void init();

    HeadIcon *m_headicon;               //头像
    QLabel *m_nickname;                 //昵称

    QString username;
    QString userid;
    QString imagePath;

    QMenu *personMenu;//点击人上的菜单
    QMenu *movetoMenu;

    QLabel *m_status;
};

#endif // LISTVIEWITEMWIDGET_H
