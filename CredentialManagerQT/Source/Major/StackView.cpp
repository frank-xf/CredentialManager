#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QStackedWidget>

#include "Credential/Credential.h"

#include "credential_qt_utils.h"

#include "Widget/ContentView.h"
#include "Major/StackView.h"

QT_BEGIN_NAMESPACE

StackView::StackView(QWidget * parent) : QStackedWidget(parent)
{
    setObjectName("StackView");
    ui_utils::SetBackgroundColor(this, Qt::white);

    m_labHint = new QLabel(this);
    m_labHint->setAlignment(Qt::AlignCenter);
    m_labHint->setMinimumSize(512, 256);
    m_labHint->setStyleSheet("QLabel{ background:transparent; color:#20A020; }");
    m_labHint->setFont(ui_utils::MakeFont());

    addWidget(m_labHint);
}

void StackView::InitCredential(const bnb::Credential & credential)
{
    auto view_credential = new CredentialView(credential, this);
    addWidget(view_credential);

    credential.PreorderTraversal([this](const bnb::platform_node& platform) {
        addWidget(new PlatformView(platform, this));

        platform.PreorderTraversal([this](const bnb::account_node& account) {
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

bool StackView::AddCredential(const bnb::Credential & credential)
{
    addWidget(new CredentialView(credential, this));

    return true;
}

bool StackView::AddPlatform(const bnb::platform_node & platform)
{
    addWidget(new PlatformView(platform, this));
    return true;
}

bool StackView::AddPlatform(const bnb::platform_node & platform, unsigned int credential_id)
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

bool StackView::AddAccount(const bnb::account_node & account)
{
    addWidget(new AccountView(account, this));
    return true;
}

bool StackView::AddAccount(const bnb::account_node & account, unsigned int platform_id)
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

bool StackView::AddProperty(const bnb::property_node & property)
{
    return true;
}

bool StackView::AddProperty(const bnb::property_node & property, unsigned int account_id)
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

bool StackView::SwitchToHint(const QString & strText)
{
    m_labHint->setText(strText);
    setCurrentWidget(m_labHint);

    return true;
}

QT_END_NAMESPACE
