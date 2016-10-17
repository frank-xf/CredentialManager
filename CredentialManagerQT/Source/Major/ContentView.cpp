#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QStackedWidget>

#include "credential_qt_utils.h"

#include "Major/ContentView.h"

ContentView::ContentView(QWidget * parent) : QStackedWidget(parent)
{
    ui_utils::SetBackgroundColor(this, Qt::white);

    setMinimumSize(400, 200);

    QHBoxLayout* pMainLayout = new QHBoxLayout;
    pMainLayout->addWidget(new QLabel("nothing", this));

    setLayout(pMainLayout);

}

void ContentView::InitCredential()
{
}

void ContentView::ClearCredential()
{
}

void ContentView::UpdatePlatform(const QString & str) const
{
	for (int i = 0; i < count(); ++i)
	{

	}



}
