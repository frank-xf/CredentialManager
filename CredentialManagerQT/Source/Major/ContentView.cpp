#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QStackedWidget>

#include "credential_qt_utils.h"
#include "credential_qt_manager.h"
#include "credential_model_manager.h"

#include "Major/ContentView.h"

#include "View/ViewBase.h"
#include "View/CredentialView.h"
#include "View/PlatformView.h"
#include "View/AccountView.h"


ContentView::ContentView(QWidget * parent) : QStackedWidget(parent)
{
    ui_utils::SetBackgroundColor(this, Qt::white);

    setMinimumSize(400, 200);

	m_labHint = new QLabel("Nothing !", this);
	m_labHint->setAlignment(Qt::AlignCenter);

	addWidget(m_labHint);

	InitCredential();
}

void ContentView::InitCredential()
{
	addWidget(new CredentialView(g_AppMgr.Model().Info(), this));

	for (auto ptr_platform = g_AppMgr.Model().Info().Tree().Head(); ptr_platform; ptr_platform = ptr_platform->m_Next)
	{
		addWidget(new PlatformView(ptr_platform->m_Pair, this));

		for (auto ptr_account = ptr_platform->m_Pair.m_Value.Head(); ptr_account; ptr_account = ptr_account->m_Next)
		{
			addWidget(new AccountView(ptr_account->m_Pair, this));
		}
	}
}

void ContentView::ClearCredential()
{
	std::vector<QWidget*> vtrWidget;
	for (int i = 0; i < count(); ++i)
	{
		QWidget* ptr = widget(i);
		if (ptr && nullptr == dynamic_cast<QLabel*>(ptr))
		{
			vtrWidget.push_back(ptr);
		}
	}

	for (auto ptr : vtrWidget)
	{
		removeWidget(ptr);
		delete ptr;
	}

	setCurrentWidget(m_labHint);
}

bool ContentView::UpdatePlatform(unsigned int credential_id, unsigned int platform_id)
{
}

bool ContentView::UpdateAccount(unsigned int platform_id, unsigned int account_id)
{
}

bool ContentView::UpdateProperty(unsigned int account_id, unsigned int property_id)
{
}

bool ContentView::RemovePlatform(unsigned int credential_id, const std::vector<unsigned int>& ids)
{
	for (auto& id : ids)
	{
		for (int i = 0; i < count(); ++i)
		{
			ViewBase* ptr = dynamic_cast<ViewBase*>(widget(i));
			if (ptr && ptr->GetID() == id)
			{
				removeWidget(ptr);
				delete ptr;
				break;
			}
		}
	}

	for (int i = 0; i < count(); ++i)
	{
		CredentialView* ptr = dynamic_cast<CredentialView*>(widget(i));
		if (ptr && ptr->GetID() == credential_id)
		{
			ptr->UpdateTable();
			return true;
		}
	}

	return false;
}

bool ContentView::RemoveAccount(unsigned int platform_id, const std::vector<unsigned int>& ids)
{
}

bool ContentView::RemoveProperty(unsigned int account_id, const std::vector<unsigned int>& ids)
{
}

bool ContentView::AddPlatform(unsigned int credential_id, bnb::platform_tree::data_type & platform)
{
	for (int i = 0; i < count(); ++i)
	{
		CredentialView* ptr = dynamic_cast<CredentialView*>(widget(i));
		if (ptr && ptr->GetID() == credential_id)
		{
			ptr->UpdateTable();
			addWidget(new PlatformView(platform, this));
			return true;
		}
	}

	return false;
}

bool ContentView::AddAccount(unsigned int platform_id, bnb::account_tree::data_type & account)
{
	for (int i = 0; i < count(); ++i)
	{
		PlatformView* ptr = dynamic_cast<PlatformView*>(widget(i));
		if (ptr && ptr->GetID() == platform_id)
		{
			ptr->UpdateTable();
			addWidget(new AccountView(account, this));
			return true;
		}
	}

	return false;
}

bool ContentView::AddProperty(unsigned int account_id, bnb::property_tree::data_type & property)
{
	for (int i = 0; i < count(); ++i)
	{
		AccountView* ptr = dynamic_cast<AccountView*>(widget(i));
		if (ptr && ptr->GetID() == account_id)
		{
			ptr->UpdateTable();
			return true;
		}
	}

	return false;
}

bool ContentView::SwitchToCredential(unsigned int id)
{
	for (int i = 0; i < count(); ++i)
	{
		CredentialView* ptr = dynamic_cast<CredentialView*>(widget(i));
		if (ptr && ptr->GetID() == id)
		{
			setCurrentWidget(ptr);
			return true;
		}
	}

	return false;
}

bool ContentView::SwitchToPlatform(unsigned int id)
{
	for (int i = 0; i < count(); ++i)
	{
		PlatformView* ptr = dynamic_cast<PlatformView*>(widget(i));
		if (ptr && ptr->GetID() == id)
		{
			setCurrentWidget(ptr);
			return true;
		}
	}

	return false;
}

bool ContentView::SwitchToAccount(unsigned int id)
{
	for (int i = 0; i < count(); ++i)
	{
		AccountView* ptr = dynamic_cast<AccountView*>(widget(i));
		if (ptr && ptr->GetID() == id)
		{
			setCurrentWidget(ptr);
			return true;
		}
	}

	return false;
}

bool ContentView::SwitchToView(unsigned int id)
{
	for (int i = 0; i < count(); ++i)
	{
		ViewBase* ptr = dynamic_cast<ViewBase*>(widget(i));
		if (ptr && ptr->GetID() == id)
		{
			setCurrentWidget(ptr);
			return true;
		}
	}

	return false;
}

bool ContentView::SwitchToHint()
{
	setCurrentWidget(m_labHint);

	return true;
}

bool ContentView::SwitchToHint(const QString & strText)
{
	m_labHint->setText(strText);

	return SwitchToHint();
}
