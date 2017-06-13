#ifndef CHATINPUT_H
#define CHATINPUT_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QToolButton;
class QTextEdit;
class QComboBox;
class QFontComboBox;
class ColorComboBox;
class QLabel;
QT_END_NAMESPACE

class ChatInput : public QWidget
{
    Q_OBJECT
public:
    explicit ChatInput(QWidget *parent = 0);


    QStringList getFontInfo();


protected:
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void keyPressEvent(QKeyEvent *event);

signals:
    void sendMsg(const QString &);
public slots:
    void btn_send_clicked();
private slots:
    void showIcons();
    void addIcon(QString path);
    void hide();

    void fontChange(int index);
    void colorChange(int index);
    void sizeChange(int index);

    void test();

private:
    void init();
    void setDefault();


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

    QLabel *label_font;
    QLabel *label_color;
    QLabel *label_size;
    QFontComboBox *cb_font;
    ColorComboBox *cb_color;
    QComboBox *cb_size;


    QString message_content;

};

#endif // CHATINPUT_H
