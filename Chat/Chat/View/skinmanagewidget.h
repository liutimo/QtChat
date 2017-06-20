#ifndef SKINMANAGEWIDGET_H
#define SKINMANAGEWIDGET_H

#include "basicwidget.h"

class QToolButton;
class QMutex;
class SkinManageWidget : public BasicWidget
{
    Q_OBJECT
public:
    static SkinManageWidget* getInstance();

protected:
    void paintEvent(QPaintEvent*);

signals:
    void updatePureColorSkin(QColor);
    void updateImageSkin(const QString &);

private slots:
    void changePureColorSkin(QColor );
    void changeImageSkin(const QString &);
    void selectImageFromFileSystem();

private:
    typedef enum _SkinType{
        PURECOLOR = 0,          //纯色
        LOCALIMAGE,             //本地图片
        NETWORKIMAGE            //网络图片
    }SkinType;
    //头像
    SkinType skinType;
    QColor color;               //纯色皮肤颜色
    QString skinPath;           //本地文件皮肤


    SkinManageWidget(QWidget *parent = Q_NULLPTR);
    void init();
    QToolButton *tbtn_selectImage;
    static QMutex* mutex;
    static SkinManageWidget* instance;
};

#endif // SKINMANAGEWIDGET_H
