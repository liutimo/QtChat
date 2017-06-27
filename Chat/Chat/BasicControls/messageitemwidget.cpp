#include "messageitemwidget.h"
#include "NetWork/httpconnect.h"
#include "numberwidget.h"
#include <QMouseEvent>
#include <QLabel>
#include <QDebug>
MessageItemWidget::MessageItemWidget(QWidget *parent) : QWidget(parent)
{
    init();
}

void MessageItemWidget::init()
{
    headicon = new QLabel(this);
    headicon->setFixedSize(32, 32);
    friendname = new QLabel(this);
    numberWidget = new NumberWidget(this);
}

void MessageItemWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
       emit itemclicked(id);

    QWidget::mousePressEvent(event);
}

void MessageItemWidget::resizeEvent(QResizeEvent *event)
{
    headicon->move(5, (height() - 32) / 2);

    friendname->resize(width() - 72, 32);
    friendname->move(40, (height() - friendname->height()) / 2);

    numberWidget->move(width() - 40, (height() - numberWidget->height()) / 2);

}

void MessageItemWidget::setId(const QString &i)
{
    id = i;
}

void MessageItemWidget::setIcon(const QString &iconpath)
{
    QUrl u(iconpath);

    if (QFile(u.fileName()).exists())
        headicon->setPixmap(QPixmap(u.fileName()).scaled(32, 32, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    else
    {
        HttpConnect *http = new HttpConnect;
        http->loadFileFormUrl(iconpath);
        connect(http, &HttpConnect::loadCompleted, this, [this, http]()
        {
           headicon->setPixmap(QPixmap(http->getFilePath()).scaled(32, 32, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        });
    }

}

void MessageItemWidget::setFriendName(const QString &_friendname)
{
    friendname->setText(_friendname);
}

void MessageItemWidget::setNumber(int number)
{
    numberWidget->setNumber(number);
}
