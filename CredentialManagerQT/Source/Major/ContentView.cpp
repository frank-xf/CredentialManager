#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QStackedWidget>

#include "credential_qt_utils.h"
#include "credential_qt_manager.h"
#include "credential_model_manager.h"

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

	for (auto ptr_platform = g_AppMgr.Model().Info().Tree().Head(); ptr_platform; ptr_platform = ptr_platform->m_Next)
	{



		/*
		QTreeWidgetItem* item_platform = AddPlatform(&ptr_platform->m_Pair.m_Key);
		for (auto ptr_account = ptr_platform->m_Pair.m_Value.Head(); ptr_account; ptr_account = ptr_account->m_Next)
		{
			QTreeWidgetItem* item_account = AddAccount(item_platform, &ptr_account->m_Pair.m_Key);
			for (auto ptr_property = ptr_account->m_Pair.m_Value.Head(); ptr_property; ptr_property = ptr_property->m_Next)
			{
				QTreeWidgetItem* item_property = AddProperty(item_account, &ptr_property->m_Pair.m_Key);
			}
		}*/
	}
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

bool ContentView::SwitchToAccount(const QString & sp, const QString & sa)
{
	return false;
}
