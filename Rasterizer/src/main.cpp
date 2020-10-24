#include "mainwindow.h"
#include <QApplication>

#include "ray.h"
#include "raster2.h"
#include <thread>


int main(int argc, char *argv[])
{
//   std::thread ru =std::thread(render(1,400,10,1.5),this);
//   if(ru.joinable()){ru.detach();}
//    ::render(1,400,10,1.5);

    ::Ras2::render();


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
