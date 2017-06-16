#ifndef MESSAGEITEMWIDGET_H
#define MESSAGEITEMWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class NumberWidget;
QT_END_NAMESPACE

class MessageItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MessageItemWidget(QWidget *parent = nullptr);

    void setIcon(const QString &iconpath);
    void setFriendName(const QString &friendname);
    void setNumber(int number = 0);
protected:
    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

signals:

public slots:

private:

    void init();

    QLabel *friendname;
    QLabel *headicon;
    NumberWidget *numberWidget;
};

#endif // MESSAGEITEMWIDGET_H
