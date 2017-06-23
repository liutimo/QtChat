#ifndef GROUPMEMBERLISTWIDGET_H
#define GROUPMEMBERLISTWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class GroupMemberLineEdit;
class ListWidget;
QT_END_NAMESPACE

class GroupMemberListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GroupMemberListWidget(QWidget *parent = nullptr);

signals:

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

public slots:

private:
    GroupMemberLineEdit *lineedit;
    ListWidget *listwidget;
};

#endif // GROUPMEMBERLISTWIDGET_H
