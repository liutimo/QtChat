#include "BasicControls/facewidget.h"
#include "chatinput.h"

#include <QToolButton>
#include <QHBoxLayout>
#include <QMenu>
#include <QDebug>
#include <QTextEdit>
#include <QTextCursor>
ChatInput::ChatInput(QWidget *parent) : QWidget(parent)
{
    init();
    setMouseTracking(true);
}

void ChatInput::init()
{
    btn_chat_input_selectFont = new QToolButton(this);
    btn_chat_input_selectFont->setObjectName("btn_chat_input_selectFont");

    btn_chat_input_icon = new QToolButton(this);
    btn_chat_input_icon->setObjectName("btn_chat_input_icon");
    connect(btn_chat_input_icon, &QToolButton::clicked, this, &ChatInput::showIcons);

    btn_chat_input_shake = new QToolButton(this);
    btn_chat_input_shake->setObjectName("btn_chat_input_shake");

    btn_chat_input_voice = new QToolButton(this);
    btn_chat_input_voice->setObjectName("btn_chat_input_voice");

    btn_chat_input_image = new QToolButton(this);
    btn_chat_input_image->setObjectName("btn_chat_input_image");

    btn_chat_input_music = new QToolButton(this);
    btn_chat_input_music->setObjectName("btn_chat_input_music");

    btn_chat_input_screenshot = new QToolButton(this);
    btn_chat_input_screenshot->setObjectName("btn_chat_input_screenshot");

    btn_chat_input_messagemanage = new QToolButton(this);
    btn_chat_input_messagemanage->setText("message record");
    btn_chat_input_messagemanage->setObjectName("btn_chat_input_messagemanage");

    te_message = new QTextEdit(this);


    btn_chat_input_send = new QToolButton(this);
    btn_chat_input_send->setText("Send");
    connect(btn_chat_input_send, &QToolButton::clicked, this, &ChatInput::btn_send_clicked);

    btn_chat_input_close = new QToolButton(this);
    btn_chat_input_close->setText("Close");

    QVBoxLayout *v_layout = new QVBoxLayout;

    QHBoxLayout *h_layout_top = new QHBoxLayout;
    h_layout_top->addWidget(btn_chat_input_selectFont);
    h_layout_top->addWidget(btn_chat_input_icon);
    h_layout_top->addWidget(btn_chat_input_shake);
    h_layout_top->addWidget(btn_chat_input_voice);
    h_layout_top->addWidget(btn_chat_input_image);
    h_layout_top->addWidget(btn_chat_input_music);
    h_layout_top->addWidget(btn_chat_input_screenshot);
    h_layout_top->addStretch();
    h_layout_top->setSpacing(10);
    h_layout_top->setContentsMargins(0,0,0,0);
    h_layout_top->addWidget(btn_chat_input_messagemanage);

    QHBoxLayout *h_layout_middle = new QHBoxLayout;
    h_layout_middle->addWidget(te_message);

    QHBoxLayout *h_layout_bottom = new QHBoxLayout;
    h_layout_bottom->addStretch();
    h_layout_bottom->addWidget(btn_chat_input_send);
    h_layout_bottom->addWidget(btn_chat_input_close);

    v_layout->addLayout(h_layout_top);
    v_layout->addLayout(h_layout_middle);
    v_layout->addLayout(h_layout_bottom);
    setLayout(v_layout);
}

void ChatInput::showIcons()
{
    FaceWidget *w = new FaceWidget();
    w->setEmotionSize(QSize(32, 32));
    w->setEmotionMovieSize(QSize(24, 24));
    w->setMaxRow(6);
    w->initTableWidget();
    for (int i = 0; i < 214; i++)
    {
        w->addEmotionItem(":/Resource/face/" + QString::number(i) +".gif", "");
    }

    connect(w, &FaceWidget::selectOneIcon, this, &ChatInput::addIcon);

    QPoint p = mapToGlobal(this->geometry().topLeft());
    w->move(p.x(), p.y() - w->height()*3 - 5);
    w->show();
}

void ChatInput::addIcon(QString path)
{
    setFocus();
    QTextImageFormat imageFormat;
    imageFormat.setName(path);
    te_message->textCursor().insertImage(imageFormat);

    show();
    this->activateWindow();
    te_message->setFocus();
}

void ChatInput::focusInEvent(QFocusEvent *event)
{
    qDebug() << "focus in";
}

void ChatInput::focusOutEvent(QFocusEvent *event)
{
    qDebug() << "focus out";
}

void ChatInput::btn_send_clicked()
{
    emit sendMsg(te_message->toPlainText()/*toHtml()*/);
}
