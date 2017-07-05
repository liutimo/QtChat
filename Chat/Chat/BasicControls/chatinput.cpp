#include "BasicControls/facewidget.h"
#include "chatinput.h"

#include <QToolButton>
#include <QHBoxLayout>
#include <QMenu>
#include <QDebug>
#include <QTextEdit>
#include <QTextCursor>
#include <QComboBox>
#include <QFontComboBox>
#include <QLabel>
#include <QKeyEvent>
#include "colorcombobox.h"

ChatInput::ChatInput(QWidget *parent) : QWidget(parent)
{
    init();
    setMouseTracking(true);
    setDefault();
}



void ChatInput::init()
{
    btn_chat_input_selectFont = new QToolButton(this);
    btn_chat_input_selectFont->setObjectName("btn_chat_input_selectFont");
    connect(btn_chat_input_selectFont, &QToolButton::clicked, this, &ChatInput::hide);

    btn_chat_input_icon = new QToolButton(this);
    btn_chat_input_icon->setObjectName("btn_chat_input_icon");
    connect(btn_chat_input_icon, &QToolButton::clicked, this, &ChatInput::showIcons);

    btn_chat_input_shake = new QToolButton(this);
    btn_chat_input_shake->setObjectName("btn_chat_input_shake");
    btn_chat_input_shake->hide();

    btn_chat_input_voice = new QToolButton(this);
    btn_chat_input_voice->setObjectName("btn_chat_input_voice");
    btn_chat_input_voice->hide();

    btn_chat_input_image = new QToolButton(this);
    btn_chat_input_image->setObjectName("btn_chat_input_image");
    btn_chat_input_image->hide();

    btn_chat_input_music = new QToolButton(this);
    btn_chat_input_music->setObjectName("btn_chat_input_music");
    btn_chat_input_music->hide();

    btn_chat_input_screenshot = new QToolButton(this);
    btn_chat_input_screenshot->setObjectName("btn_chat_input_screenshot");
    btn_chat_input_screenshot->hide();

    btn_chat_input_messagemanage = new QToolButton(this);
    btn_chat_input_messagemanage->setText("聊天记录");
    btn_chat_input_messagemanage->setObjectName("btn_chat_input_messagemanage");
    btn_chat_input_messagemanage->hide();

    label_font = new QLabel("字体:", this);
    label_color = new QLabel("颜色:", this);
    label_size = new QLabel("大小:", this);

    cb_font = new QFontComboBox(this);
    cb_font->setEditable(false);


    cb_color = new ColorComboBox(this);
    cb_color->setEditable(false);
    cb_color->setCurrentIndex(7); //默认黑色

    cb_size = new QComboBox(this);
    cb_size->setEditable(false);


    te_message = new QTextEdit(this);
    te_message->setStyleSheet("background-color:rgba(255,255,255,0)");
    connect(te_message, &QTextEdit::textChanged, this, &ChatInput::test);

    btn_chat_input_send = new QToolButton(this);
    btn_chat_input_send->setText("发送");
    btn_chat_input_send->setStyleSheet("background-color:#00B0F0;");
    connect(btn_chat_input_send, &QToolButton::clicked, this, &ChatInput::btn_send_clicked);

    btn_chat_input_close = new QToolButton(this);
    btn_chat_input_close->setText("关闭");
    btn_chat_input_close->setStyleSheet("background-color:#EB3F2F;");

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


    QHBoxLayout *h_layout_top_hide = new QHBoxLayout;
    h_layout_top_hide->addWidget(label_font);
    h_layout_top_hide->addWidget(cb_font);

    h_layout_top_hide->addWidget(label_size);
    h_layout_top_hide->addWidget(cb_size);

    h_layout_top_hide->addWidget(label_color);
    h_layout_top_hide->addWidget(cb_color);

    h_layout_top_hide->addStretch();
    h_layout_top_hide->setSpacing(5);


    QHBoxLayout *h_layout_middle = new QHBoxLayout;
    h_layout_middle->addWidget(te_message);

    QHBoxLayout *h_layout_bottom = new QHBoxLayout;
    h_layout_bottom->addStretch();
    h_layout_bottom->addWidget(btn_chat_input_send);
    h_layout_bottom->addWidget(btn_chat_input_close);


    v_layout->addLayout(h_layout_top_hide);
    v_layout->addLayout(h_layout_top);
    v_layout->addLayout(h_layout_middle);
    v_layout->addLayout(h_layout_bottom);
    setLayout(v_layout);

    this->setStyleSheet("QToolButton{background-color: rgba(255, 255, 255, 0);}");

    connect(btn_chat_input_close, &QToolButton::clicked, this, [this](){
       emit closed();
    });
}

void ChatInput::test()
{

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

void ChatInput::setDefault()
{

    connect(cb_font, SIGNAL(currentIndexChanged(int)), this, SLOT(fontChange(int)));
    connect(cb_color, SIGNAL(currentIndexChanged(int)), this,SLOT(colorChange(int)));
    connect(cb_size, SIGNAL(currentIndexChanged(int)), this,SLOT(sizeChange(int)));


    QStringList sizes;
    sizes << "8" << "10" << "12" << "14" << "16" << "18" << "20" << "22" << "24";

    cb_size->addItems(sizes);

    cb_size->setCurrentIndex(3);

    cb_color->hide();
    cb_font->hide();
    cb_size->hide();
    label_color->hide();
    label_font->hide();
    label_size->hide();

}

void ChatInput::focusInEvent(QFocusEvent *event)
{

}

void ChatInput::focusOutEvent(QFocusEvent *event)
{

}

void ChatInput::btn_send_clicked()
{
    QString message = te_message->toPlainText();
    QString p("<p.*>(.*)<\/p>");
    QRegExp rx(p);
    rx.indexIn(te_message->document()->toHtml());
    message = rx.cap(1);

    emit sendMsg(message);
    te_message->clear();
}

void ChatInput::hide()
{
    if(cb_color->isHidden())
    {
        cb_color->show();
        cb_font->show();
        cb_size->show();
        label_color->show();
        label_font->show();
        label_size->show();
    }
    else
    {
        cb_color->hide();
        cb_font->hide();
        cb_size->hide();
        label_color->hide();
        label_font->hide();
        label_size->hide();
    }
}


void ChatInput::fontChange(int index)
{
    te_message->setFont(QFont(cb_font->currentText(), cb_size->currentText().toInt()));
    QString str = te_message->document()->toPlainText();
    te_message->clear();
    te_message->setText(str);
}

void ChatInput::colorChange(int index)
{
    te_message->setTextColor(cb_color->itemData(index, Qt::DecorationRole).toString());
    QString str = te_message->document()->toPlainText();
    te_message->clear();
    te_message->setText(str);
}

void ChatInput::sizeChange(int index)
{
    te_message->setFont(QFont(cb_font->currentText(), cb_size->currentText().toInt()));
    QString str = te_message->document()->toPlainText();
    te_message->clear();
    te_message->setText(str);
}

QStringList ChatInput::getFontInfo()
{
    QStringList fontinfo;
    fontinfo << cb_font->currentText() << cb_size->currentText() << cb_color->itemData(cb_color->currentIndex(), Qt::DecorationRole).toString();
    return fontinfo;
}
void ChatInput::keyReleaseEvent(QKeyEvent *event)
{

}

void ChatInput::keyPressEvent(QKeyEvent *event)
{

}
