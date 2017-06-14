#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include "basicwidget.h"
#include "BasicControls/chatinput.h"

class HeadIcon;
class QPushButton;

class ChatWidget : public BasicWidget
{
    Q_OBJECT
public:
    ChatWidget(QWidget *parent =Q_NULLPTR);
    void setUserName(const QString &username);
    void setUserid(const QString &userid);
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void setMessage(const QString&);
    void showMessage(const QString &msg, const QString &color, const QString &size, const QString &family);

private:
    void init();

    HeadIcon *headIcon;
    QLabel *lb_username;

    QPushButton *btn_chat_settings;
    QPushButton *btn_chat_close;
    QPushButton *btn_chat_min;
    QPushButton *btn_chat_max;

    QTextEdit *textedit;

    ChatInput *chatinput;

    QString userid;

};

#endif // CHATWIDGET_H
