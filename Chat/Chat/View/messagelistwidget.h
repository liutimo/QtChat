#ifndef MESSAGELISTWIDGET_H
#define MESSAGELISTWIDGET_H

#include <QWidget>

#include "basicwidget.h"

QT_BEGIN_NAMESPACE
class QListWidget;
QT_END_NAMESPACE

class MessageListWidget : public BasicWidget
{
    Q_OBJECT
public:
    explicit MessageListWidget(QWidget *parent = nullptr);
    int getHeight();

signals:
    void nonewmessage();
public slots:
    void updateMessage();
    void showChatWidget(const QString);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QListWidget *listwidget;

};

#endif // MESSAGELISTWIDGET_H
