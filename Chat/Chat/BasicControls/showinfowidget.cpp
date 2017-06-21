#include "showinfowidget.h"
#include <QUrl>
#include <QFile>
#include <QLabel>
#include <QPixmap>
#include "BasicControls/headicon.h"
#include "NetWork/httpconnect.h"
ShowInfoWidget::ShowInfoWidget(QWidget *parent) : BasicWidget(parent)
{
    setFixedSize(300, 480);
    init();
}


void ShowInfoWidget::init()
{

    headicon = new HeadIcon(this);
    headicon->setPixmap(QPixmap(":/timg (1).jpg"));
    headicon->setFixedSize(80, 80);
    headicon->move((width() - 80) / 2, 50);
    headicon->setObjectName("mainwidget_headicon");

    userid = new QLabel("帐号:", this);
    userid->setFixedSize(65, 30);
    userid->move(40, 130);
    userid->setAlignment(Qt::AlignRight);

    username = new QLabel("昵称:", this);
    username->setFixedSize(65, 30);
    username->move(40, 170);
    username->setAlignment(Qt::AlignRight);

    sex = new QLabel("性别:", this);
    sex->setFixedSize(65, 30);
    sex->move(40, 210);
    sex->setAlignment(Qt::AlignRight);

    birthofdate = new QLabel("生日:", this);
    birthofdate->setFixedSize(65, 30);
    birthofdate->move(40, 250);
    birthofdate->setAlignment(Qt::AlignRight);

    mail = new QLabel("邮箱:", this);
    mail->setFixedSize(65, 30);
    mail->move(40, 290);
    mail->setAlignment(Qt::AlignRight);

    mobile = new QLabel("手机号:", this);
    mobile->setFixedSize(65, 30);
    mobile->move(40, 330);
    mobile->setAlignment(Qt::AlignRight);

    personalizedsignature = new QLabel("个性签名:", this);
    personalizedsignature->setFixedSize(65, 30);
    personalizedsignature->move(40, 370);
    personalizedsignature->setAlignment(Qt::AlignRight);


    luserid = new QLabel( this);
    luserid->setFixedSize(150, 30);
    luserid->move(110, 130);
    luserid->setAlignment(Qt::AlignLeft);

    lusername = new QLabel( this);
    lusername->setFixedSize(150, 30);
    lusername->move(110, 170);
    lusername->setAlignment(Qt::AlignLeft);

    lsex = new QLabel(this);
    lsex->setFixedSize(150, 210);
    lsex->move(110, 210);
    lsex->setAlignment(Qt::AlignLeft);

    lbirthofdate = new QLabel(this);
    lbirthofdate->setFixedSize(150, 30);
    lbirthofdate->move(110, 250);
    lbirthofdate->setAlignment(Qt::AlignLeft);


    lmail = new QLabel(this);
    lmail->setFixedSize(150, 30);
    lmail->move(110, 290);
    lmail->setAlignment(Qt::AlignLeft);

    lmobile = new QLabel(this);
    lmobile->setFixedSize(150, 30);
    lmobile->move(110, 330);
    lmobile->setAlignment(Qt::AlignLeft);


    lpersonalizedsignature = new QLabel(this);
    lpersonalizedsignature->setFixedSize(150, 60);
    lpersonalizedsignature->move(110, 370);
    lpersonalizedsignature->setAlignment(Qt::AlignLeft);

}

void ShowInfoWidget::setInfo(const QVector<QString> &s)
{
    luserid->setText(s[0]);
    lusername->setText(s[1]);
    lsex->setText(s[2]);
    lbirthofdate->setText(s[3]);
    lmail->setText(s[4]);
    lmobile->setText(s[5]);
    lpersonalizedsignature->setText(s[6]);

    if(QFile(QUrl(s.last()).fileName()).exists())
        headicon->setPixmap(QPixmap(QUrl(s.last()).fileName()));
    else
    {
        HttpConnect *http = new HttpConnect();
        http->loadFileFormUrl(s.last());

        connect(http, &HttpConnect::loadCompleted, this, [this, http](){
            headicon->setPixmap(QPixmap(http->getFilePath()));
        });
    }
}
