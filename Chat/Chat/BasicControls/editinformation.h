#ifndef EDITINFORMATION_H
#define EDITINFORMATION_H

#include "View/basicwidget.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QComboBox;
class QTextEdit;
class QPushButton;
QT_END_NAMESPACE

class EditInformation : public BasicWidget
{
    Q_OBJECT
public:
    EditInformation(QWidget *parent = Q_NULLPTR);
protected:
    void resizeEvent(QResizeEvent *event);

private:
    void init();

    QLabel *lb_username;
    QLabel *lb_sex;
    QLabel *lb_birth;
    QLabel *lb_mobile;
    QLabel *lb_mail;
    QLabel *lb_ps;

    QLineEdit *le_username;
    QLineEdit *le_mobile;
    QLineEdit *le_mail;
    QLineEdit *le_birth;

    QTextEdit *te_ps;

    QComboBox *cb_sex;

    QPushButton *btn_cancel;
    QPushButton *btn_save;
};

#endif // EDITINFORMATION_H
