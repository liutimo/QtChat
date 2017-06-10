#include "myemotionwidget.h"
#include <QLabel>
#include <QMovie>
#include <QDebug>
#include <windows.h>
#include <QFile>
#include <QHeaderView>
#include "myemotionitemwidget.h"

MyEmotionWidget::MyEmotionWidget(QWidget *parent)
	: QTableWidget(parent)
	, m_tableRow(0)
	, m_tableColumn(0)
	, m_preRow(0)
	, m_preColumn(0)
	, m_maxRow(6)
	, m_emotionSize(QSize(0 , 0))
	, m_emotionMovieSize(QSize(0 , 0))
{
	loadStyleSheet();
// 	initWindow();
// 	installEventFilter(this);
}

MyEmotionWidget::~MyEmotionWidget()
{

}

void MyEmotionWidget::loadStyleSheet()
{
	QFile file(":/Resources/QSS/myemotion.css");
	file.open(QFile::ReadOnly);
	QString strSheet = file.readAll();
	QString styleSheet = this->styleSheet();
	styleSheet += strSheet;
	this->setStyleSheet(styleSheet);
}

void MyEmotionWidget::initWindow()
{
//	this->setWindowFlags(Qt::FramelessWindowHint);	
// 	setRowAndColumn(4, 4);
// 	setEmotionSize(QSize(32, 32));
// 	initTableWidget();
// 	QString path = "E:\\QtPractice3_8\\InterfaceTest_8_8\\InterfaceTest_8_30\\InterfaceTest\\Resources\\QQexpression\\%1.gif";
// 	for (int i = 0; i < 10; i++)
// 	{
// 		addEmotionItem(path.arg(i + 1), "");
// 	}
}

void MyEmotionWidget::initTableWidget()
{
	this->setFocusPolicy(Qt::NoFocus);
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);
	this->setRowCount(m_tableRow);
	this->setColumnCount(m_tableColumn);

	this->horizontalHeader()->setVisible(false);
	this->horizontalHeader()->setDefaultSectionSize(m_emotionSize.width());

	this->verticalHeader()->setVisible(false);
	this->verticalHeader()->setDefaultSectionSize(m_emotionSize.height());
	
	if (m_tableRow > m_maxRow)
	{
		this->setFixedHeight(m_emotionSize.height()*m_maxRow + 2);
		this->setFixedWidth(m_emotionSize.width()*m_tableColumn + 20);
	}
	else
	{
		this->setFixedHeight(m_emotionSize.height()*m_tableRow + 2);
		this->setFixedWidth(m_emotionSize.width()*m_tableColumn + 2);
	}
}

void MyEmotionWidget::setRowAndColumn(int row, int column)
{
	m_tableRow = row;
	m_tableColumn = column;
}

void MyEmotionWidget::setEmotionSize(QSize emotionSize)
{
	m_emotionSize = emotionSize;
}

void MyEmotionWidget::setEmotionMovieSize(QSize emotionMovieSize)
{
	m_emotionMovieSize = emotionMovieSize;
}

void MyEmotionWidget::setMaxRow(int maxRow)
{
	m_maxRow = maxRow;
}

void MyEmotionWidget::addEmotionItem(QString fileName , QString toolTip)
{
	int row = m_emotionList.size() / this->columnCount();
	int column = m_emotionList.size() % this->columnCount();

	QTableWidgetItem* tableWidgetItem = new QTableWidgetItem;
	tableWidgetItem->setToolTip(toolTip);
	this->setItem(row, column, tableWidgetItem);

	MyEmotionItemWidget* emotionIcon = new MyEmotionItemWidget(fileName , m_emotionMovieSize);

// 	QLabel* emotionIcon = new QLabel;
// 	emotionIcon->setContentsMargins(3, 3, 3, 3);
// 	emotionIcon->setStyleSheet("QLabel:hover{border: 1px solid rgb(111, 156, 207);\
// 								background: rgba(255, 255, 255, 200);}");
// 	QMovie* movie = new QMovie;
// 	movie->setScaledSize(QSize(m_emotionMovieSize.width(), m_emotionMovieSize.height()));
// 	movie->setFileName(fileName);
// 	// start and stop 后图片显示正常;
// 	movie->start();
// 	movie->stop();
// 	emotionIcon->setMovie(movie);
	this->setCellWidget(row, column, emotionIcon);
	m_emotionList.push_back(fileName);
}

void MyEmotionWidget::showDynamicEmotion(QPoint cursorPos)
{
	QPoint point = cursorPos - this->pos(); 
	QTableWidgetItem* tableWidgetItem = this->itemAt(point);
	if (tableWidgetItem != NULL)
	{
		//如果获取到的item不为空说明此刻鼠标所在的区域在tableWidget的某个item中，
		//并且如果鼠标从一个表情移动到一个其他未设置item的单元格或者移出tableWidget区域
		//在同一个item中移动则不做任何操作;
		if (m_preRow != tableWidgetItem->row() || m_preColumn != tableWidgetItem->column())
		{
			QLabel* preEmotion = (QLabel*)this->cellWidget(m_preRow, m_preColumn);
			if (preEmotion != NULL)
			{
				QMovie* preMoive = preEmotion->movie();
				preMoive->jumpToFrame(0);
				preMoive->stop();
				preEmotion->setContentsMargins(3, 3, 3, 3);
			}

			QLabel* curEmotion = (QLabel*)this->cellWidget(tableWidgetItem->row(), tableWidgetItem->column());
			if (curEmotion != NULL)
			{
				curEmotion->setContentsMargins(4, 2, 2, 4);
				QMovie* curMoive = curEmotion->movie();
				curMoive->start();
				m_preRow = tableWidgetItem->row();
				m_preColumn = tableWidgetItem->column();
			}
		}
	}
	else
	{
		//如果获取到的item为空说明此刻鼠标所在的区域不在tableWidget的items中，
		//并且如果鼠标刚从一个动态表情移动到一个其他未设置item的单元格或者移出tableWidget区域，
		//则根据上一个表情的位置获取到item并停止正在动态显示的表情，并将m_preRow、m_preColumn置为-1，
		//以免下次再次进入到此分支进行重复操作;
		QTableWidgetItem* tableWidgetItem = this->item(m_preRow, m_preColumn);
		if (tableWidgetItem != NULL)
		{
			QLabel* preEmotion = (QLabel*)this->cellWidget(m_preRow, m_preColumn);
			if (preEmotion != NULL)
			{
				preEmotion->setContentsMargins(3, 3, 3, 3);
				QMovie* preMoive = preEmotion->movie();
				preMoive->jumpToFrame(0);
				preMoive->stop();
				m_preRow = -1;
				m_preColumn = -1;
			}
		}
	}
}

bool MyEmotionWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
// 	MSG* msg = reinterpret_cast<MSG*>(message);
// 	switch (msg->message)
// 	{
// 	case WM_MOUSEMOVE:
// 	{
// 		QPoint point = QCursor::pos() - this->pos();
// 		QTableWidgetItem* tableWidgetItem = this->itemAt(point);
// 		if (tableWidgetItem != NULL)
// 		{
// 			//如果获取到的item不为空说明此刻鼠标所在的区域在tableWidget的某个item中，
// 			//并且如果鼠标从一个表情移动到一个其他未设置item的单元格或者移出tableWidget区域
// 			//在同一个item中移动则不做任何操作;
// 			if (m_preRow != tableWidgetItem->row() || m_preColumn != tableWidgetItem->column())
// 			{
// 				QLabel* preEmotion = (QLabel*)this->cellWidget(m_preRow, m_preColumn);
// 				if (preEmotion != NULL)
// 				{
// 					QMovie* preMoive = preEmotion->movie();
// 					preMoive->jumpToFrame(0);
// 					preMoive->stop();
// 					preEmotion->setContentsMargins(3, 3, 3, 3);
// 				}
// 
// 				QLabel* curEmotion = (QLabel*)this->cellWidget(tableWidgetItem->row(), tableWidgetItem->column());
// 				if (curEmotion != NULL)
// 				{
// 					curEmotion->setContentsMargins(4, 2, 2, 4);
// 					QMovie* curMoive = curEmotion->movie();
// 					curMoive->start();
// 					m_preRow = tableWidgetItem->row();
// 					m_preColumn = tableWidgetItem->column();
// 				}
// 			}
// 		}
// 		else
// 		{
// 			//如果获取到的item为空说明此刻鼠标所在的区域不在tableWidget的items中，
// 			//并且如果鼠标刚从一个动态表情移动到一个其他未设置item的单元格或者移出tableWidget区域，
// 			//则根据上一个表情的位置获取到item并停止正在动态显示的表情，并将m_preRow、m_preColumn置为-1，
// 			//以免下次再次进入到此分支进行重复操作;
// 			QTableWidgetItem* tableWidgetItem = this->item(m_preRow, m_preColumn);
// 			if (tableWidgetItem != NULL)
// 			{
// 				QLabel* preEmotion = (QLabel*)this->cellWidget(m_preRow, m_preColumn);
// 				if (preEmotion != NULL)
// 				{
// 					preEmotion->setContentsMargins(3, 3, 3, 3);
// 					QMovie* preMoive = preEmotion->movie();
// 					preMoive->jumpToFrame(0);
// 					preMoive->stop();
// 					m_preRow = -1;
// 					m_preColumn = -1;
// 				}
// 			}
// 		}
// 	}
// 	}
	return __super::nativeEvent(eventType, message, result);
}

bool MyEmotionWidget::eventFilter(QObject* object, QEvent* event)
{
// 	if (QEvent::WindowActivate == event->type())
// 	{
// 		qDebug() << "WindowActivate_____________";
// 	}
// 	else if (QEvent::WindowDeactivate == event->type())
// 	{
// 		this->hide();
// 		qDebug() << "WindowDeactivate___________";
// 	}

	return __super::eventFilter(object, event);
}