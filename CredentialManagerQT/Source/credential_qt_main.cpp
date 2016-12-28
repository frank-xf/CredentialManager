#include <QtWidgets/QApplication>

QT_BEGIN_NAMESPACE
    void Init();
QT_END_NAMESPACE

int main(int argc, char *argv[])
{
    QT_USE_NAMESPACE

    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);

    QApplication app(argc, argv);

    Init();

	return app.exec();
}
