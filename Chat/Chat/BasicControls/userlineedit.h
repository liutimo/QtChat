#ifndef USERLINEEDIT_H
#define USERLINEEDIT_H

#include <QLineEdit>
class QLabel;
class QPushButton;

class UserLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    UserLineEdit(QWidget *parent = Q_NULLPTR);

protected:
    void resizeEvent(QResizeEvent *);
    void focusOutEvent(QFocusEvent *e);
    void focusInEvent(QFocusEvent *e);
    void keyPressEvent(QKeyEvent *);
private:
    void init();
};

#endif // USERLINEEDIT_H
