#include "mainwindow.h"
#include <QApplication>
#include "ray.h"

int main(int argc, char *argv[])
{

    ::render(1,400,10,1.5);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
