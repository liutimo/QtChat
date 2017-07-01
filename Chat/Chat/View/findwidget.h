#ifndef FINDWIDGET_H
#define FINDWIDGET_H


#include "basicwidget.h"


QT_BEGIN_NAMESPACE
class SearchLineEdit;
class QPushButton;
class QLabel;
class QMovie;
class HeadIcon;
QT_END_NAMESPACE

class FindWidget : public BasicWidget
{
    Q_OBJECT
public:
    explicit FindWidget(QWidget *parent = nullptr);

signals:

private slots:
    void searchUser();
    void addFriend();
    void sendRequest(const QString &, const QString&);
private:
    void init();
    void parseUserInfo(const QByteArray&);

    SearchLineEdit *lineedit;
    QPushButton *btn_searach;
    QLabel *label;
    QMovie *movie;

    HeadIcon *headicon;
    QLabel *userid;
    QLabel *username;

    QPushButton *add_button;


};

#endif // FINDWIDGET_H
