#ifndef CHATINPUT_H
#define CHATINPUT_H

#include <QWidget>

class QToolButton;
class QTextEdit;

class ChatInput : public QWidget
{
    Q_OBJECT
public:
    explicit ChatInput(QWidget *parent = 0);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
signals:
    void sendMsg(const QString &);
public slots:
    void btn_send_clicked();
private slots:
    void showIcons();
    void addIcon(QString path);
private:
    void init();

    QToolButton *btn_chat_input_selectFont;
    QToolButton *btn_chat_input_icon;
    QToolButton *btn_chat_input_shake;
    QToolButton *btn_chat_input_voice;
    QToolButton *btn_chat_input_image;
    QToolButton *btn_chat_input_music;
    QToolButton *btn_chat_input_screenshot;
    QToolButton *btn_chat_input_messagemanage;

    QTextEdit *te_message;

    QToolButton *btn_chat_input_send;
    QToolButton *btn_chat_input_close;
};

#endif // CHATINPUT_H
