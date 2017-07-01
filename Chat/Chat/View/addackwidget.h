#ifndef ADDACKWIDGET_H
#define ADDACKWIDGET_H

#include "basicwidget.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QComboBox;
class QTextEdit;
class QPushButton;
QT_END_NAMESPACE

class AddAckWidget : public BasicWidget
{
    Q_OBJECT
public:
    explicit AddAckWidget(QWidget *parent = nullptr);
    void setText(const QString &userid, const QString &message);

public slots:

private:

    void init();

    QLabel* hite1;
    QLabel *hite2;
    QLabel *hite3;
    QLabel *hite4;

    QComboBox *groups;

    QPushButton *ok;
    QPushButton *cancel;

    QString userid;
};

#endif // ADDACKWIDGET_H
