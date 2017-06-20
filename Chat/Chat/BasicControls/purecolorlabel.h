#ifndef PURECOLORLABEL_H
#define PURECOLORLABEL_H

#include <QLabel>
#include <QWidget>

class PureColorLabel : public QLabel
{
    Q_OBJECT
public:
    PureColorLabel(QWidget *parent = Q_NULLPTR);
    void setColor(QColor c);
    QColor getColor();

signals:
    void selectedColor(QColor);

protected:
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
private:
    QColor color;
};

#endif // PURECOLORLABEL_H
