#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>
class QLabel;
class QPushButton;

class LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    LineEdit(QWidget *parent = Q_NULLPTR);

protected:
    void resizeEvent(QResizeEvent *);
    void focusOutEvent(QFocusEvent *e);
    void focusInEvent(QFocusEvent *e);

private:
    void init();

    QLabel *pix_label; // 显示图片
    QPushButton *btn_clear;
};

#endif // LINEEDIT_H
