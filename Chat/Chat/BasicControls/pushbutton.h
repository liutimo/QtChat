#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H


#include <QPushButton>

class PushButton : public QPushButton
{
    Q_OBJECT
public:
    PushButton(QWidget *parent = Q_NULLPTR);

public slots:
    void setBackgroundColor(QColor color);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QColor backgroundcolor;
};

#endif // PUSHBUTTON_H
