
#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>

#include "Major/MainView.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainView view;
    view.show();

    return app.exec();
}
