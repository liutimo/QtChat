#include "skinmanagewidget.h"
#include "Setting/rwsetting.h"
#include "BasicControls/purecolorlabel.h"
#include "BasicControls/imageskinlabel.h"

#include <QMutex>
#include <QPainter>
#include <QSettings>
#include <QToolButton>
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
            connect(l, &ImageSkinLabel::selecteImage, this, &SkinManageWidget::changeImageSkin);
        }
    }


    for(int i = 0; i < 15; ++i)
    {
        PureColorLabel *l = new PureColorLabel(this);
        l->setColor(QColor(qrand() % 256, qrand() % 256, qrand() % 256));
        l->resize(40, 40);
        l->move(i*41, 505);
        connect(l, &PureColorLabel::selectedColor, this, &SkinManageWidget::changePureColorSkin);
    }

    tbtn_selectImage = new QToolButton(this);
    tbtn_selectImage->setObjectName("tbtn_selectImage");
    tbtn_selectImage->setFixedSize(72, 24);
    tbtn_selectImage->move(5, 555);

    QSettings *setting = RWSetting::getInstance()->getSetting();
    SkinType skintype = (SkinType)setting->value("SkinType").toInt();

    switch (skintype) {
    case PURECOLOR:
        changePureColorSkin((setting->value("SKINCOLOR")).value<QColor>());
        break;
    case LOCALIMAGE:
        changeImageSkin(setting->value("SKINIMAGE").toString());
    default:
        break;
    }

    connect(tbtn_selectImage, &QToolButton::clicked, this, &SkinManageWidget::selectImageFromFileSystem);
}

void SkinManageWidget::changePureColorSkin(QColor _color)
{
    RWSetting::getInstance()->getSetting()->setValue("SkinType", PURECOLOR);
    RWSetting::getInstance()->getSetting()->setValue("SKINCOLOR", _color);
    skinType = PURECOLOR;
    color = _color;
    update();
//    setTitleBackgroundColor(color);
    emit updatePureColorSkin(color);
}

void SkinManageWidget::changeImageSkin(const QString& path)
{
    RWSetting::getInstance()->getSetting()->setValue("SkinType", LOCALIMAGE);
    RWSetting::getInstance()->getSetting()->setValue("SKINIMAGE", path);
    skinType = LOCALIMAGE;
    skinPath = path;
    update();
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

void SkinManageWidget::paintEvent(QPaintEvent*event)
{
    QPainter p(this);
    p.setPen(Qt::NoPen);

    switch (skinType) {
    case PURECOLOR:
    {
        p.setBrush(color);
        p.drawRect(0, 0, width(), height());
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
