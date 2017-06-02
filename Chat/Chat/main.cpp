#include "widget.h"
#include "View/loginwidget.h"
#include "DataBase/database.h"
#include "BasicControls/headicon.h"
#include "BasicControls/listviewitemwidget.h"
#include "BasicControls/listwidget.h"
#include "View/mainwidget.h"
#include <QApplication>
#include <QFile>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile f(":/style.qss");
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    a.setStyleSheet(f.readAll());
    //qDebug() << f.readAll();
    LoginWidget w;
    //ListWidget w;
    //MainWidget w;
    w.show();

    return a.exec();
}
