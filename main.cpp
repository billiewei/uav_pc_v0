#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;

    //window.setLayout(mainLayout);

 //   window.setWindowTitle(QApplication::translate("UAV Control pc v0.0"));
    window.setWindowTitle(QApplication::translate("toplevel", "UAV Control"));
    window.resize(800, 500);
    window.show();

    return app.exec();
}
