#ifndef MYEMOTIONITEMWIDGET_H
#define MYEMOTIONITEMWIDGET_H

#include <QLabel>

class FaceItemWidget : public QLabel
{
	Q_OBJECT

public:
    FaceItemWidget(QString fileName, QSize emotionMoiveSize);
    ~FaceItemWidget();

private:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);

private:
	
};

#endif // MYEMOTIONITEMWIDGET_H
