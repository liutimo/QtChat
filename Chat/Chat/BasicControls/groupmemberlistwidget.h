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
    void setList(const QVector<QStringList> &vec);
signals:
    void selectedOneFriend(const QString &userid, const QString &username);
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void selectOneFriend(const QModelIndex &index);

public slots:

private:
    GroupMemberLineEdit *lineedit;
    ListWidget *listwidget;
};

#endif // GROUPMEMBERLISTWIDGET_H
