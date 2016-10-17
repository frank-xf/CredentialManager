#include <QtWidgets/QApplication>

#include "credential_qt_manager.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	g_AppMgr.Initialize();

	return app.exec();
}
