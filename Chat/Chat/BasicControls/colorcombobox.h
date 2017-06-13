#ifndef COLORCOMBOBOX_H
#define COLORCOMBOBOX_H

#include <QComboBox>

QT_BEGIN_NAMESPACE
class QColor;
class QWidget;
QT_END_NAMESPACE

class ColorComboBox : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor USER true)

public:
    ColorComboBox(QWidget *widget = 0);

public:
    QColor color() const;
    void setColor(QColor c);

private:
    void populateList();
};
#endif // COLORCOMBOBOX_H
