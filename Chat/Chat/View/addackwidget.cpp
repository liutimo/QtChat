#include "addackwidget.h"

#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include "NetWork/connecttoserver.h"
#include "DataBase/database.h"
AddAckWidget::AddAckWidget(QWidget *parent) : BasicWidget(parent)
{
    setWidgetTitle("好友请求");
    init();
    setFixedSize(300, 260);
}

void AddAckWidget::init()
{
    hite1 = new QLabel(this);
    hite1->move(20, 35);
    hite1->setFixedSize(300, 30);

    hite4 = new QLabel(this);
    hite4->setText("选择分组:");
    hite4->move(20, 65);

    groups = new QComboBox(this);
    groups->setFixedSize(150, 30);
    groups->move(90, 65);

    QStringList list = DataBase::getInstance()->getGroup();
    groups->addItems(list);


    hite2 = new QLabel("验证信息:", this);
    hite2->move(20, 95);

    hite3 = new QLabel(this);
    hite3->move(26, 105);
    hite3->setFixedSize(200, 60);


    ok = new QPushButton("同意", this);
    ok->setFixedSize(120, 35);
    ok->move(20, 200);
    ok->setStyleSheet("QPushButton{border-radius:5px; color:white; background:#00B2EE;}"
                      "QPushButton:hover{background: #00BFFF;}"
                      "QPushButton:pressed{background:#009ACD;}");

    cancel = new QPushButton("拒绝", this);
    cancel->setFixedSize(120, 35);
    cancel->move(160, 200);
    cancel->setStyleSheet("QPushButton{border-radius:5px; color:white; background: #EE2C2C;}"
                      "QPushButton:hover{background:#EE0000;}"
                      "QPushButton:pressed{background:#CC2C2C;}");
    connect(cancel, &QPushButton::clicked, this, [this]()
    {
        ConnectToServer::getInstance()->sendAddFriendResult(userid, "", 2);
        close();
    });

    connect(ok, &QPushButton::clicked, this, [this]()
    {
        ConnectToServer::getInstance()->sendAddFriendResult(userid, groups->currentText(), 1);
        close();
    });
}
void AddAckWidget::setText(const QString &userid, const QString &message)
{
    this->userid = userid;
    hite1->setText(QString("用户<span style=\"color:red;\">%1</span>请求加你为好友").arg(userid));
    hite3->setText(message);
}
