#ifndef LISTVIEWITEMWIDGET_H
#define LISTVIEWITEMWIDGET_H

#include <QWidget>


class QLabel;
class HeadIcon;

class ListViewItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ListViewItemWidget(QWidget *parent = 0);
    void setUserinfo(const QString &username, const QString &signature);



signals:

public slots:

protected:
    void resizeEvent(QResizeEvent *);

private:
    void init();

    HeadIcon *m_headicon;               //头像
    QLabel *m_nickname;                 //昵称
};

#endif // LISTVIEWITEMWIDGET_H
