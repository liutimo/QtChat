#ifndef NUMBERWIDGET_H
#define NUMBERWIDGET_H

#include <QLabel>

class NumberWidget : public QLabel
{
public:
    NumberWidget(QWidget *parent = Q_NULLPTR);
    void setNumber(const int num = 0);

protected:
    void paintEvent(QPaintEvent *event);

private:
    int number = 0;
};

#endif // NUMBERWIDGET_H
