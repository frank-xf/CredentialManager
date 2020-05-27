
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

#include "views/MainView.h"

int main(int argc, char *argv[])
{
    QT_USE_NAMESPACE

    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QApplication app(argc, argv);

    MainView* _viewMain = new MainView;
    _viewMain->Init();

	return app.exec();
}
