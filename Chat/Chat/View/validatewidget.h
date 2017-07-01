#ifndef VALIDATEWIDGET_H
#define VALIDATEWIDGET_H


#include "basicwidget.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QComboBox;
class QTextEdit;
class QPushButton;
QT_END_NAMESPACE

class ValidateWidget : public BasicWidget
{
    Q_OBJECT
public:
    explicit ValidateWidget(QWidget *parent = nullptr);

signals:
    void info(const QString&, const QString&);
public slots:

private:

    void init();

    QLabel* hite1;
    QTextEdit *content;
    QLabel* hite2;
    QComboBox *groups;

    QPushButton *ok;
    QPushButton *cancel;
};

#endif // VALIDATEWIDGET_H
