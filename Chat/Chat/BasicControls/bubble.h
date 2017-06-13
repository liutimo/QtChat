#ifndef BUBBLE_H
#define BUBBLE_H

#include <QLabel>

class Bubble :
    public QLabel
{
    Q_OBJECT
public:
    typedef enum _BubbleOrientation
    {
        Left = 0,
        Right = 1
    }BubbleOrientation;

    Bubble(QWidget *parent = 0);
    ~Bubble();

    void setBubbleOrientatio(BubbleOrientation orientation);
    void setText(const QString &text);

protected:
    void paintEvent(QPaintEvent *event);

private:
    BubbleOrientation orientation;
};


#endif // BUBBLE_H
