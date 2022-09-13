#include "JJY_PROJECT.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    JJY_PROJECT w;
    w.show();
    return a.exec();
}
