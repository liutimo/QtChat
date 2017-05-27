#ifndef IMAGESKINLABEL_H
#define IMAGESKINLABEL_H

#include <QLabel>

class ImageSkinLabel : public QLabel
{
    Q_OBJECT
public:
    ImageSkinLabel(const QString &, QWidget *parent = Q_NULLPTR);

signals:
    void selecteImage(QString);

protected:
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);

private:
    QString filepath;
};

#endif // IMAGESKINLABEL_H
