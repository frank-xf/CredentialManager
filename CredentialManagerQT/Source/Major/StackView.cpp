#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QStackedWidget>

#include "credential_qt_utils.h"
#include "credential_qt_manager.h"
#include "credential_model_manager.h"

#include "Major/StackView.h"
#include "Widget/ContentView.h"


StackView::StackView(QWidget * parent) : QStackedWidget(parent)
{
    ui_utils::SetBackgroundColor(this, Qt::white);

    setMinimumSize(400, 200);

    m_labHint = new QLabel("Nothing !", this);
    m_labHint->setAlignment(Qt::AlignCenter);

    addWidget(m_labHint);
}

void StackView::InitCredential()
{
    auto view_credential = new CredentialView(g_AppMgr.Model().Info(), this);
    addWidget(view_credential);

    g_AppMgr.Model().Info().Tree().Foreach([this](const bnb::platform_tree::data_type& platform) {
        addWidget(new PlatformView(platform, this));
    
        platform.m_Value.Foreach([this](const bnb::account_tree::data_type& account) {
            addWidget(new AccountView(account, this));
        });
    });

    setCurrentWidget(view_credential);
}

void StackView::ClearCredential()
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

bool StackView::UpdateCredential(unsigned int credential_id)
{
    for (int i = 0; i < count(); ++i)
    {
        CredentialView* ptr = dynamic_cast<CredentialView*>(widget(i));
        if (ptr && ptr->GetID() == credential_id)
        {
            ptr->UpdateInfo();
            return true;
        }
    }

    return false;
}

bool StackView::UpdatePlatform(unsigned int credential_id, unsigned int platform_id)
{
    bool bResult1 = false, bResult2 = false;

    for (int i = 0; i < count(); ++i)
    {
        CredentialView* ptr = dynamic_cast<CredentialView*>(widget(i));
        if (ptr && ptr->GetID() == credential_id)
        {
            ptr->UpdateTable(platform_id);
            bResult1 = true;
            break;
        }
    }

    for (int i = 0; i < count(); ++i)
    {
        PlatformView* ptr = dynamic_cast<PlatformView*>(widget(i));
        if (ptr && ptr->GetID() == platform_id)
        {
            ptr->UpdateInfo();
            bResult2 = true;
            break;
        }
    }

    return bResult1 && bResult2;
}

bool StackView::UpdateAccount(unsigned int platform_id, unsigned int account_id)
{
    bool bResult1 = false, bResult2 = false;

    for (int i = 0; i < count(); ++i)
    {
        PlatformView* ptr = dynamic_cast<PlatformView*>(widget(i));
        if (ptr && ptr->GetID() == platform_id)
        {
            ptr->UpdateTable(account_id);
            bResult1 = true;
            break;
        }
    }

    for (int i = 0; i < count(); ++i)
    {
        AccountView* ptr = dynamic_cast<AccountView*>(widget(i));
        if (ptr && ptr->GetID() == account_id)
        {
            ptr->UpdateInfo();
            bResult2 = true;
            break;
        }
    }

    return bResult1 && bResult2;
}

bool StackView::UpdateProperty(unsigned int account_id, unsigned int property_id)
{
    for (int i = 0; i < count(); ++i)
    {
        AccountView* ptr = dynamic_cast<AccountView*>(widget(i));
        if (ptr && ptr->GetID() == account_id)
        {
            ptr->UpdateTable(property_id);
            return true;
        }
    }

    return false;
}

bool StackView::RemovePlatform(unsigned int credential_id, const std::vector<unsigned int>& ids)
{
    RemoveView(ids);

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

bool StackView::RemoveAccount(unsigned int platform_id, const std::vector<unsigned int>& ids)
{
    RemoveView(ids);

    for (int i = 0; i < count(); ++i)
    {
        PlatformView* ptr = dynamic_cast<PlatformView*>(widget(i));
        if (ptr && ptr->GetID() == platform_id)
        {
            ptr->UpdateTable();
            return true;
        }
    }

    return false;
}

bool StackView::RemoveProperty(unsigned int account_id, const std::vector<unsigned int>& ids)
{
    RemoveView(ids);

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

unsigned int StackView::RemoveView(const std::vector<unsigned int>& ids)
{
    unsigned int nCount = 0;
    for (auto& id : ids)
    {
        for (int i = 0; i < count(); ++i)
        {
            BaseView* ptr = dynamic_cast<BaseView*>(widget(i));
            if (ptr && ptr->GetID() == id)
            {
                removeWidget(ptr);
                delete ptr;
                ++nCount;
                break;
            }
        }
    }

    return nCount;
}

bool StackView::AddPlatform(unsigned int credential_id, const bnb::platform_tree::data_type & platform)
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

bool StackView::AddAccount(unsigned int platform_id, const bnb::account_tree::data_type & account)
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

bool StackView::AddProperty(unsigned int account_id, const bnb::property_tree::data_type & property)
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

bool StackView::SwitchToCredential(unsigned int id)
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

bool StackView::SwitchToPlatform(unsigned int id)
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

bool StackView::SwitchToAccount(unsigned int id)
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

bool StackView::SwitchToView(unsigned int id)
{
    for (int i = 0; i < count(); ++i)
    {
        BaseView* ptr = dynamic_cast<BaseView*>(widget(i));
        if (ptr && ptr->GetID() == id)
        {
            setCurrentWidget(ptr);
            return true;
        }
    }

    return false;
}

bool StackView::SwitchToHint()
{
    setCurrentWidget(m_labHint);

    return true;
}

bool StackView::SwitchToHint(const QString & strText)
{
    m_labHint->setText(strText);

    return SwitchToHint();
}
