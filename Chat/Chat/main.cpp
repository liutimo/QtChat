#include "widget.h"
#include "View/loginwidget.h"
#include "BasicControls/headicon.h"
#include <QApplication>
#include "DataBase/database.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWidget w;
//    HeadIcon w;
//    w.setPixmap(QPixmap(":/timg (1).jpg"));

    w.show();

    return a.exec();
}
