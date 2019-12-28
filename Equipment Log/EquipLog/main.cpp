#include "dash.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dash w;
    w.show();

    return a.exec();
}
