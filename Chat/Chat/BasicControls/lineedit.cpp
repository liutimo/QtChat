#include "lineedit.h"
#include "DataBase/database.h"

#include <QPixmap>
#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QPushButton>

LineEdit::LineEdit(QWidget *parent ) : QLineEdit(parent)
{
    init();

    setStyleSheet("QLineEdit{border:0px;background-color: rgba(255, 255, 255, 200);color:rgb(170,170,170);text-align:center;font:15px}");

    connect(this, &LineEdit::textChanged, this, &LineEdit::textChange);
}
void LineEdit::init()
{
    resize(200, 30);
    setPlaceholderText("搜索");

    pix_label = new QLabel(this);
    pix_label->setPixmap(QPixmap(":/Resource/mainwidget/search_icon.png"));
    pix_label->move(4, 0);

    btn_clear = new QPushButton(this);
    btn_clear->setFixedSize(16, 16);
    btn_clear->setObjectName("main_search_btn_clear");
    btn_clear->setToolTip("清除");
    btn_clear->hide();

    connect(btn_clear, &QPushButton::clicked, this, [this](){
        btn_clear->hide();
        this->clear();
        setStyleSheet("QLineEdit{border:0px;background-color: rgba(255, 255, 255, 200);color:rgb(170,170,170);text-align:center;font:15px}");
    });
}


void LineEdit::resizeEvent(QResizeEvent *event)
{
    pix_label->resize(22, height());

    btn_clear->move(width() - 24, (height() - 16) / 2);

    setTextMargins(26, 0, 32, 0);
}

void LineEdit::focusOutEvent(QFocusEvent *e)
{
    QLineEdit::focusOutEvent(e);
    //btn_clear->hide();
    setStyleSheet("QLineEdit{border:0px;background-color: rgba(255, 255, 255, 200);color:rgb(170,170,170);text-align:center;font:15px}");
    emit LineEditFocusOut();
}

void LineEdit::focusInEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);
    btn_clear->show();
    setStyleSheet("QLineEdit{border:0px;background-color: rgba(255, 255, 255, 255);color:rgb(170,170,170);text-align:center;font:15px}");
    emit LineEditFocusIn();
}

void LineEdit::textChange(const QString &text)
{
    if(!text.isEmpty())
    emit searachResult(DataBase::getInstance()->searachFriend(text));
}

void LineEdit::hideButton()
{
    btn_clear->hide();
}
