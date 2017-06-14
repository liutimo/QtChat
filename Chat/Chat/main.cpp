#include "BasicControls/chatinput.h"
#include "BasicControls/facewidget.h"
#include "View/loginwidget.h"
#include "View/chatwidget.h"
#include "View/mainwidget.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile f(":/style.qss");
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    a.setStyleSheet(f.readAll());

    LoginWidget w;

    w.show();
    return a.exec();
}
