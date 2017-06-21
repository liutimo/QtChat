#ifndef RECENTCHATITEMWIDGET_H
#define RECENTCHATITEMWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class HeadIcon;
class QMenu;
QT_END_NAMESPACE

class RecentChatItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RecentChatItemWidget(QWidget *parent = 0);
    void setUserinfo(const QString &userid, const QString &username, const QString &signature);
    void setImage(const QString& url);
    void setMessage(const QString &message);

    QString getUsername();
    QString getUserid();
signals:
    void updateListWidget();
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
    QLabel *m_nickname;                 //昵称+备注
    QLabel *m_chatmessage;

    QString username;
    QString userid;
    QString imagePath;

    QMenu *personMenu;//点击人上的菜单
    QMenu *movetoMenu;
};


#endif // RECENTCHATITEMWIDGET_H
