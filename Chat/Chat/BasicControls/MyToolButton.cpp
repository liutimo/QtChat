#include "mytoolbutton.h"
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include <QMenu>
#include <QMouseEvent>
#include <QMessageBox>

MyToolButton::MyToolButton(QWidget *parent , QString strIcon)
	: QToolButton(parent)
	, m_strIcon(strIcon)
{
	initControl();
	initMenu();
}

MyToolButton::~MyToolButton()
{

}

void MyToolButton::initControl()
{	
	this->setCheckable(true);
	this->setPopupMode(QToolButton::MenuButtonPopup);
	this->setFixedSize(QSize(45, 34));
	this->setStyleSheet(QString("QToolButton\
						{\
						 	border-image: url(%1) 0 0 68 0;\
						}\
						QToolButton:hover\
						{\
							border-image: url(%1) 34 0 34 0;\
						}\
						QToolButton:pressed,QToolButton:checked\
						{\
							border-image: url(%1) 68 0 0 0;\
						}\
						QToolButton::menu-arrow\
						{\
							border:none\
						}\
						QToolButton::menu-indicator{\
						image: none;\
						}\
						QToolButton::menu-button\
						{\
						background-image:url(:/Resources/toolButton_arrow_tranparent.png);\
						}\
						QToolButton::menu-button:pressed\
						{\
						background-image:url(:/Resources/toolButton_arrow.png);\
						}").arg(m_strIcon));
	connect(this, SIGNAL(pressed()), this, SLOT(onMousePress()));
	connect(this, SIGNAL(clicked()), this, SLOT(onMouseClicked()));
	connect(this, SIGNAL(released()), this, SLOT(onMouseRelease()));
	connect(this, SIGNAL(signalToolMenuClicked()), this, SLOT(onToolMenuClicked()));
	
}

void MyToolButton::initMenu()
{
 	m_menu = new QMenu(this);
 	QAction* actionOne = m_menu->addAction("one");
 	QAction* actionTwo = m_menu->addAction("two");
}

void MyToolButton::enterEvent(QEvent *event)
{
	if (this->isChecked())
	{
		showMenuArrow();
	}
	QToolButton::enterEvent(event);
}

void MyToolButton::leaveEvent(QEvent *event)
{
	hideMenuArrow();
	QToolButton::leaveEvent(event);
}

void MyToolButton::mousePressEvent(QMouseEvent *event)
{
	QStyleOptionToolButton styleIOption;
	initStyleOption(&styleIOption);
	if (Qt::LeftButton == event->button() && MenuButtonPopup == popupMode())
	{
		// 获取QToolButton右边菜单按钮的区域;
		QRect popupButtonRect = style()->subControlRect(QStyle::CC_ToolButton, &styleIOption, QStyle::SC_ToolButtonMenu, this);
		if (popupButtonRect.isValid() && popupButtonRect.contains(event->pos()))
		{
			if (this->isChecked())
			{
				//m_menu->exec(event->globalPos());
				emit signalToolMenuClicked();
			}

			
		}
	}
	qDebug() << "this is mousePressEvent Fuction";
	QToolButton::mousePressEvent(event);
}

void MyToolButton::mouseReleaseEvent(QMouseEvent *event)
{
	qDebug() << "this is mouseReleaseEvent Fuction";
	QToolButton::mouseReleaseEvent(event);
}

void MyToolButton::nextCheckState()
{
	//	setChecked(!isChecked());			// nextCheckState方法 原来的实现
	this->setChecked(true);
	if (isChecked())
	{
		showMenuArrow();
	//	this->setMenu(m_menu);
	}
	else
	{
		hideMenuArrow();
	//	this->setMenu(NULL);
	}
	qDebug() << "this is nextCheckState Fuction";
}

void MyToolButton::checkStateSet()
{
	bool isCheck = this->isChecked();
	qDebug() << "this is checkStateSet Fuction";
}

void MyToolButton::onMousePress()
{
	qDebug() << "this is onMousePress Fuction";
}

void MyToolButton::showMenuArrow()
{
	QString styleSheet = this->styleSheet();
	styleSheet += "QToolButton::menu-button\
					{\
					background-image:url(:/Resources/toolButton_arrow.png);\
					}";
	this->setStyleSheet(styleSheet);
}

void MyToolButton::hideMenuArrow()
{
	QString styleSheet = this->styleSheet();
	styleSheet += "QToolButton::menu-button\
					{\
					background-image:url(:/Resources/toolButton_arrow_tranparent.png);\
					}";
	this->setStyleSheet(styleSheet);
}

void MyToolButton::setIcon(QString strIcon)
{
	m_strIcon = strIcon;
}

void MyToolButton::onMouseClicked()
{
	qDebug() << "this is onMouseClicked Fuction";
}

void MyToolButton::onMouseRelease()
{
	qDebug() << "this is onMouseRelease Fuction";
}

void MyToolButton::onToolMenuClicked()
{
	QMessageBox msgBox;
	msgBox.setText("You Have Clicked MenuButton !");
	msgBox.exec();
}