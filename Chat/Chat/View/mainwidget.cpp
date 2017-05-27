#include "mainwidget.h"
#include "skinmanagewidget.h"
#include "BasicControls/headicon.h"

#include <QPushButton>
#include <QResizeEvent>
#include <QPainter>
#include <QTime>
#include <QDebug>


MainWidget::MainWidget(QWidget *parent) : BasicWidget(parent),
    skinType(PURECOLOR),
    color(QColor(40,138,221))
{
    init();
    setAutoFillBackground(true);
    setMinimumWidth(300);
    resize(300, 600);

    setWidgetTitle("这是一个主窗口");

}

void MainWidget::init()
{
    btn_mini = new QPushButton(this);
    btn_mini->setToolTip("最小化");
    btn_mini->setFixedSize(28, 26);
    btn_mini->setObjectName("btn_mini");

    btn_skin = new QPushButton(this);
    btn_skin->setToolTip("更换皮肤");
    btn_skin->setFixedSize(28, 26);
    btn_skin->setObjectName("btn_skin");
    btn_skin->setMouseTracking(false);

    headIcon = new HeadIcon(this);
    headIcon->setPixmap(QPixmap(":/timg (1).jpg"));
    headIcon->setFixedSize(64, 64);
    headIcon->move(15, 40);
    headIcon->setObjectName("mainwidget_headicon");

    connect(btn_skin, &QPushButton::clicked, this, &MainWidget::showSkinManageWidget);

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
        p.drawRect(0, 0, width(), 153);
        //emit changeBackGround(color);
        break;
    }
    case LOCALIMAGE:
    {
        p.drawPixmap(0, 0, width(), height(),
                     QPixmap(skinPath).copy(0,0,width(), height()));
        break;
    }
    default:
        break;
    }
}

void MainWidget::showSkinManageWidget()
{
    SkinManageWidget *skin = SkinManageWidget::getInstance();
    skin->show();
    skin->move(this->geometry().right() + 1, this->geometry().y());
    connect(skin, &SkinManageWidget::updatePureColorSkin, this, &MainWidget::changePureColorSkin);
    connect(skin, &SkinManageWidget::updateImageSkin, this, &MainWidget::changeImageSkin);
}

void MainWidget::changePureColorSkin(QColor _color)
{
    skinType = PURECOLOR;
    color = _color;
    update();
}

void MainWidget::changeImageSkin(const QString &path)
{
    skinType = LOCALIMAGE;
    skinPath = path;
    update();
}
