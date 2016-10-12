#include <QtWidgets/QWidget>

#include "credential_view_manager.h"

#include "Major/MainView.h"

void CredentialViewManager::Initialize()
{
	m_viewMain = new MainView;
	m_viewMain->show();
}

QWidget * CredentialViewManager::Main()
{
	return m_viewMain;
}
