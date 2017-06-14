#include "userlineedit.h"

#include "lineedit.h"
#include <QPixmap>
#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QPushButton>
#include <QKeyEvent>
UserLineEdit::UserLineEdit(QWidget *parent ) : QLineEdit(parent)
{
    init();

    setStyleSheet("QLineEdit{border:0px;background-color: rgba(255, 255, 255, 0);color:rgb(0,0, 0);text-align:center;font:15px}");


}
void UserLineEdit::init()
{
    resize(200, 30);
}


void UserLineEdit::resizeEvent(QResizeEvent *event)
{

}

void UserLineEdit::focusOutEvent(QFocusEvent *e)
{
    QLineEdit::focusOutEvent(e);
    setStyleSheet("QLineEdit{border:0px;background-color: rgba(255, 255, 255, 0);color:rgb(0,0,0);text-align:center;font:15px}");
}

void UserLineEdit::focusInEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);
    setStyleSheet("QLineEdit{border:0px;background-color: rgba(255, 255, 255, 255);color:rgb(0,0,0);text-align:center;font:15px}");
}

void UserLineEdit::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == 16777220 || e->key() == 16777221) {
        setStyleSheet("QLineEdit{border:0px;background-color: rgba(255, 255, 255, 0);color:rgb(0,0,0);text-align:center;font:15px}");
        this->clearFocus();
    }
}
