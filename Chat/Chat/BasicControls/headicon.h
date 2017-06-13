#ifndef HEADICON_H
#define HEADICON_H

#include <QLabel>
#include <QPixmap>

class HeadIcon : public QLabel
{
    Q_OBJECT
public:
    HeadIcon(QWidget *parent = Q_NULLPTR);
    void setPixmap(const QPixmap &);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QPixmap pixmap;
};

#endif // HEADICON_H
