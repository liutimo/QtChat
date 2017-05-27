#include "skinmanagewidget.h"

#include "BasicControls/purecolorlabel.h"
#include "BasicControls/imageskinlabel.h"
#include <QToolButton>
#include <QMutex>
#include <QFileDialog>
#include <QDesktopServices>
SkinManageWidget::SkinManageWidget(QWidget *parent) : BasicWidget(parent)
{
    setWidgetTitle("更换皮肤");
    setAdjustmentSize(false);
    setBackgroundColor(Qt::white);
    resize(614, 480);
    init();
}

QMutex* SkinManageWidget::mutex = new QMutex();
SkinManageWidget* SkinManageWidget::instance = NULL;

void SkinManageWidget::init()
{

    int s = 1;

    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 6; ++j)
        {
            //400 * 600 -> 100 * 150
            ImageSkinLabel *l = new ImageSkinLabel(":/Resource/skinmanage/" +
                                                   QString::number(s++) + ".jpg", this);
            l->resize(100, 150);
            l->move(j * 101 + 4,  i * 151 + 29);
            connect(l, &ImageSkinLabel::selecteImage, this, &SkinManageWidget::changImageSkin);
        }
    }


    for(int i = 0; i < 15; ++i)
    {
        PureColorLabel *l = new PureColorLabel(this);
        l->setColor(QColor(qrand() % 256, qrand() % 256, qrand() % 256));
        l->resize(40, 40);
        l->move(i*41, 505);
        connect(l, &PureColorLabel::selecteColor, this, &SkinManageWidget::changPureColorSkin);
    }

    tbtn_selectImage = new QToolButton(this);
    tbtn_selectImage->setObjectName("tbtn_selectImage");
    tbtn_selectImage->setFixedSize(72, 24);
    tbtn_selectImage->move(5, 555);


    connect(tbtn_selectImage, &QToolButton::clicked, this, &SkinManageWidget::selectImageFromFileSystem);
}

void SkinManageWidget::changPureColorSkin(QColor color)
{
    setTitleBackgroundColor(color);
    emit updatePureColorSkin(color);
}

void SkinManageWidget::changImageSkin(QString path)
{
    //setTitleBackgroundColor(color);
    emit updateImageSkin(path);
}

void SkinManageWidget::selectImageFromFileSystem()
{  
    //系统存放图片的文件夹
    QString path =  QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);

    QString fileName = QFileDialog::getOpenFileName(this, tr("打开"),
                                                    path, tr("图像文件 (*.png *.xpm *.jpg)"));


    emit updateImageSkin(fileName);
}

SkinManageWidget* SkinManageWidget::getInstance()
{
    mutex->lock();

    if (instance == NULL)
        instance = new SkinManageWidget();

    mutex->unlock();

    return instance;
}

