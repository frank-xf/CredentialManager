
#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>

#include "Major/CredentialMainView.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CredentialMainView view;
    view.show();

    return app.exec();
}
