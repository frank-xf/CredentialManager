#include <QtWidgets/QApplication>

QT_BEGIN_NAMESPACE
    void Init();
QT_END_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);

	QApplication app(argc, argv);

    QT_PREPEND_NAMESPACE(Init)();

	return app.exec();
}
