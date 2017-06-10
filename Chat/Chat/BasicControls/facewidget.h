#ifndef MYEMOTIONWIDGET_H
#define MYEMOTIONWIDGET_H

#include <QWidget>
#include <QTableWidget>

class FaceWidget : public QTableWidget
{
	Q_OBJECT

public:
    FaceWidget(QWidget *parent = 0);
    ~FaceWidget();

	void setRowAndColumn(int row , int column);
	void setEmotionSize(QSize emotionSize);
	void setEmotionMovieSize(QSize emotionMovieSize);
	void setMaxRow(int maxRow);
	void addEmotionItem(QString fileName, QString toolTip = "");

    void initTableWidget();

	void showDynamicEmotion(QPoint cursorPos);
private:
    void loadStyleSheet();


protected:
    void mousePressEvent(QMouseEvent *event);
    void focusOutEvent(QFocusEvent *event);
private slots:

signals:
    void selectOneIcon(QString path);

private:

	int m_preRow;
	int m_preColumn;
	int m_tableRow;
	int m_tableColumn;
	int m_maxRow;
	QSize m_emotionSize;
	QSize m_emotionMovieSize;
	QList<QString> m_emotionList;
};

#endif // MYEMOTIONWIDGET_H
