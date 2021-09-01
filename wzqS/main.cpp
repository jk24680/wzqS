#include "wzqS.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    wzqS w;
    w.show();
    return a.exec();
}
