#include "validatewidget.h"

#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>

#include "DataBase/database.h"
#include "allvariable.h"

ValidateWidget::ValidateWidget(QWidget *parent) : BasicWidget(parent)
{
    init();
    setFixedSize(300, 260);
}

void ValidateWidget::init()
{
    hite1 = new QLabel(this);
    hite1->setText("请输入验证消息:");
    hite1->move(20, 35);

    QTextEdit *content = new QTextEdit(this);
    content->setFixedSize(260, 60);
    content->move(20, 55);
    content->setPlaceholderText("我是" + AllVariable::getLoginUserId());
    hite2 = new QLabel(this);
    hite2->setText("请选择分组:");
    hite2->move(20, 120);

    groups = new QComboBox(this);
    groups->setFixedSize(220, 30);
    groups->move(20, 145);

    QStringList list = DataBase::getInstance()->getGroup();

    for(QString str : list)
    {
        groups->addItem(str);
    }

    ok = new QPushButton("发送", this);
    ok->setFixedSize(120, 35);
    ok->move(20, 200);
    ok->setStyleSheet("QPushButton{border-radius:5px; color:white; background:#00B2EE;}"
                      "QPushButton:hover{background: #00BFFF;}"
                      "QPushButton:pressed{background:#009ACD;}");

    cancel = new QPushButton("取消", this);
    cancel->setFixedSize(120, 35);
    cancel->move(160, 200);
    cancel->setStyleSheet("QPushButton{border-radius:5px; color:white; background: #EE2C2C;}"
                      "QPushButton:hover{background:#EE0000;}"
                      "QPushButton:pressed{background:#CC2C2C;}");
    connect(cancel, &QPushButton::clicked, this, [this](){close();});

    connect(ok, &QPushButton::clicked, this, [this, content]()
    {
        emit info(content->document()->toPlainText(), groups->currentText());
        this->close();
    });
}
