#include "colorcombobox.h"


ColorComboBox::ColorComboBox(QWidget *widget) : QComboBox(widget)
{
    populateList();
}

//! [0]
QColor ColorComboBox::color() const
{
    return qvariant_cast<QColor>(itemData(currentIndex(), Qt::DecorationRole));
}
//! [0]

//! [1]
void ColorComboBox::setColor(QColor color)
{
    setCurrentIndex(findData(color, int(Qt::DecorationRole)));
}
//! [1]

//! [2]
void ColorComboBox::populateList()
{
    QStringList colorNames = QColor::colorNames();

    for (int i = 0; i < colorNames.size(); ++i) {
        QColor color(colorNames[i]);
        insertItem(i, colorNames[i]);
        setItemData(i, color, Qt::DecorationRole);
    }
}
