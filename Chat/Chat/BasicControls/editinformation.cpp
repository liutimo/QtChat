#include "editinformation.h"

#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QValidator>
EditInformation::EditInformation(QWidget *parent) : BasicWidget(parent)
{
    init();
    setFixedSize(360, 490);
    setWidgetTitle("修改资料");
}

void EditInformation::init()
{
    lb_username = new QLabel("用户名",this);
    le_username = new QLineEdit(this);
    le_username->setMaxLength(10);

    lb_sex = new QLabel("性别", this);

    lb_birth = new QLabel("生日", this);

    lb_mobile = new QLabel("手机号", this);

    lb_mail = new QLabel("邮箱", this);

    lb_ps = new QLabel("个性签名", this);

    QRegExp re1("\\d{11}");
    le_mobile = new QLineEdit(this);
    le_mobile->setValidator(new QRegExpValidator(re1));

    QRegExp re2("^[a-zA-Z0-9_-]+@[a-zA-Z0-9_-]+(\.[a-zA-Z0-9_-]+)+$");
    le_mail = new  QLineEdit(this);
    le_mail->setValidator(new QRegExpValidator(re2));

    le_birth = new QLineEdit(this);

    te_ps = new QTextEdit(this);

    cb_sex = new QComboBox(this);
    cb_sex->addItem("男");
    cb_sex->addItem("女");

    btn_cancel = new QPushButton("取消", this);
    btn_cancel->setFixedSize(60, 30);
    connect(btn_cancel, &QPushButton::clicked, this, [this](){this->close();});

    btn_save = new QPushButton("保存", this);
    btn_save->setFixedSize(60, 30);

}

void EditInformation::resizeEvent(QResizeEvent *event)
{
    BasicWidget::resizeEvent(event);

    int h = 15;
    lb_username->move(10, 35);
    le_username->move(80, 35);
    le_username->resize(250, 30);

    lb_sex->move(10, 80);
    cb_sex->move(80, 80);
    cb_sex->resize(250, 30);

    lb_birth->move(10, 125);
    le_birth->move(80, 125);
    le_birth->resize(250, 30);

    lb_mobile->move(10, 170);
    le_mobile->move(80, 170);
    le_mobile->resize(250, 30);

    lb_mail->move(10, 215);
    le_mail->move(80, 215);
    le_mail->resize(250, 30);

    lb_ps->move(10, 260);
    te_ps->move(80, 260);
    te_ps->resize(250, 150);

    btn_save->move(200, 430);
    btn_cancel->move(270, 430);
}
