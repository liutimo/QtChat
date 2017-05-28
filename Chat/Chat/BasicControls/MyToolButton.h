#ifndef MYTOOLBUTTON_H
#define MYTOOLBUTTON_H

#include <QToolButton>

class MyToolButton : public QToolButton
{
	Q_OBJECT

public:
	MyToolButton(QWidget *parent = 0 , QString strIcon = "");
	~MyToolButton();

	void setIcon(QString strIcon);

signals:
	void signalToolMenuClicked();

private:
	void initControl();
	void initMenu();

	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void nextCheckState();
	void checkStateSet();

	void showMenuArrow();
	void hideMenuArrow();
private slots:
	void onMousePress();	
	void onMouseClicked();
	void onMouseRelease();

	void onToolMenuClicked();

private:
	QMenu* m_menu;
	QString m_strIcon;
};

#endif // MYTOOLBUTTON_H
