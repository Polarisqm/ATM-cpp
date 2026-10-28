#include "atmwindow.h"
#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("ATM-cpp");
    QCoreApplication::setOrganizationName("ATM-cpp");
    QCoreApplication::setApplicationVersion("1.0.0");

    ATMWindow window;
    window.show();
    return app.exec();
}
