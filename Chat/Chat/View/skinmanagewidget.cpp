#include "skinmanagewidget.h"

#include "BasicControls/purecolorlabel.h"

#include <QToolButton>
#include <QMutex>

SkinManageWidget::SkinManageWidget(QWidget *parent) : BasicWidget(parent)
{
    setWidgetTitle("更换皮肤");
    setAdjustmentSize(false);

    resize(610, 480);

    init();

}

QMutex* SkinManageWidget::mutex = new QMutex();
SkinManageWidget* SkinManageWidget::instance = NULL;

void SkinManageWidget::init()
{
    for(int i = 1; i < 9; ++i)
    {
        for(int j = 0; j < 10; ++j)
        {
            PureColorLabel *l = new PureColorLabel(this);
            l->setColor(QColor(qrand() % 256, qrand() % 256, qrand() % 256));
            l->resize(60, 60);
            l->move(j*61, i*61);

            connect(l, &PureColorLabel::selecteColor, this, &SkinManageWidget::changSkin);
        }
    }

    tbtn_selectImage = new QToolButton(this);
    tbtn_selectImage->setObjectName("tbtn_selectImage");
    tbtn_selectImage->setFixedSize(72, 24);
    tbtn_selectImage->move(5, 555);
}

void SkinManageWidget::changSkin(QColor color)
{
    emit updatePureColorSkin(color);
}

SkinManageWidget* SkinManageWidget::getInstance()
{
    mutex->lock();

    if (instance == NULL)
        instance = new SkinManageWidget();

    mutex->unlock();

    return instance;
}
