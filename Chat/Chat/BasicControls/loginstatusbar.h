#ifndef LOGINSTATUSBAR_H
#define LOGINSTATUSBAR_H

#include <QWidget>

class QToolButton;
class QLabel;

class LoginStatusBar : public QWidget
{
    Q_OBJECT
public:
    explicit LoginStatusBar(QWidget *parent = 0);
    void setInfo(const QString &userid);

signals:
    void hide_status();
public slots:
    void btn_hide_oncliked();

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    void init();

    QToolButton *btn_hide;
    QLabel *lb_showinfo;
    QLabel *lb_showicon;

};

#endif // LOGINSTATUSBAR_H
