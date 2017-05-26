#include "mainwidget.h"
#include "BasicControls/pushbutton.h"
#include <QResizeEvent>
#include <QPainter>
#include <QTime>
#include <QDebug>
MainWidget::MainWidget(QWidget *parent) : BasicWidget(parent),
    skinType(LOCALIMAGE),
    color(QColor(40,138,221))
{
    init();
    setAutoFillBackground(true);
    setMinimumWidth(300);
    resize(300, 600);

}

void MainWidget::init()
{
    btn_mini = new PushButton(this);
    btn_mini->setToolTip("最小化");
    btn_mini->setFixedSize(28, 26);
    btn_mini->setObjectName("btn_mini");

    btn_skin = new PushButton(this);
    btn_skin->setToolTip("更换皮肤");
    btn_skin->setFixedSize(28, 26);
    btn_skin->setObjectName("btn_skin");
    btn_skin->setMouseTracking(false);

    connect(btn_skin, &PushButton::clicked, this, &MainWidget::changeSkin);
    connect(this, MainWidget::changeBackGround, btn_mini, &PushButton::setBackgroundColor);
    connect(this, MainWidget::changeBackGround, btn_skin, &PushButton::setBackgroundColor);
    connect(this, MainWidget::changeBackGround, btn_close, &PushButton::setBackgroundColor);
}

void MainWidget::resizeEvent(QResizeEvent *event)
{
    BasicWidget::resizeEvent(event);
    btn_mini->move(width() - btn_mini->width() - 28, 0);
    btn_skin->move(width() - btn_skin->width() - 56, 0);
}

void MainWidget::paintEvent(QPaintEvent*event)
{
    QPainter p(this);
    p.setPen(Qt::NoPen);

    switch (skinType) {
    case PURECOLOR:
    {
        p.setBrush(color);
        p.drawRect(0, 0, width(), 105);
        emit changeBackGround(color);
        qDebug() << "重绘" << color.toRgb();
        break;
    }
    case LOCALIMAGE:
    {
        p.drawPixmap(0, 0, width(), height(),
                     /*QPixmap(":/Resource/background_test.jpg").scaled(
                         this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)*/QPixmap(":/Resource/background_test.jpg").copy(0,0,width(), height()));
        break;
    }
    default:
        break;
    }
}

void MainWidget::changeSkin()
{
    color = QColor(qrand() % 256, qrand() % 256, qrand() % 256);
    update();
}
