#ifndef LISTVIEWITEMWIDGET_H
#define LISTVIEWITEMWIDGET_H

#include <QWidget>

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
    QString getUsername();
    QString getUserid();
signals:

public slots:

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

    QMenu *personMenu;//点击人上的菜单
    QMenu *movetoMenu;
};

#endif // LISTVIEWITEMWIDGET_H
