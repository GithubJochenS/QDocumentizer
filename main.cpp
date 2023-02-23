#include "Documentizer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Documentizer w(nullptr,&a);
    w.show();
    return a.exec();
}
