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

signals:
    void updatePureColorSkin(QColor);
    void updateImageSkin(const QString &);

private slots:
    void changPureColorSkin(QColor);
    void changImageSkin(QString);
    void selectImageFromFileSystem();

private:
    SkinManageWidget(QWidget *parent = Q_NULLPTR);
    void init();
    QToolButton *tbtn_selectImage;
    static QMutex* mutex;
    static SkinManageWidget* instance;
};

#endif // SKINMANAGEWIDGET_H
