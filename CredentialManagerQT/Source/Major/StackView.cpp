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

void StackView::AddCredential(const bnb::Credential & credential)
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

bool StackView::AddPlatform(const bnb::platform_node & platform)
{
    if (auto ptr_credential = dynamic_cast<bnb::Credential*>(platform.GetParent()))
    {
        for (int i = 0; i < count(); ++i)
        {
            CredentialView* ptr = dynamic_cast<CredentialView*>(widget(i));
            if (ptr && ptr->GetID() == ptr_credential->GetID())
            {
                ptr->UpdateTable();
                ptr->UpdateInfo();
                break;
            }
        }

        addWidget(new PlatformView(platform, this));
        return true;
    }

    return false;
}

bool StackView::AddAccount(const bnb::account_node & account)
{
    if (auto ptr_platform = dynamic_cast<bnb::platform_node*>(account.GetParent()))
    {
        if (auto ptr_credential = dynamic_cast<bnb::Credential*>(ptr_platform->GetParent()))
        {
            UpdateView<PlatformView>(ptr_credential->GetID(), ptr_platform->GetID());
            addWidget(new AccountView(account, this));
            return true;
        }
    }

    return false;
}

bool StackView::AddProperty(const bnb::property_node & property)
{
    if (auto ptr_account = dynamic_cast<bnb::account_node*>(property.GetParent()))
        if (auto ptr_platform = dynamic_cast<bnb::platform_node*>(ptr_account->GetParent()))
            if (auto ptr_credential = dynamic_cast<bnb::Credential*>(ptr_platform->GetParent()))
                return UpdateView<AccountView>(ptr_credential->GetID(), ptr_account->GetID());

    return false;
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
        if (!bResult1)
        {
            PlatformView* ptr = dynamic_cast<PlatformView*>(widget(i));
            if (ptr && ptr->GetID() == platform_id)
            {
                ptr->UpdateInfo();
                bResult1 = true;
            }
        }
        if (!bResult2)
        {
            CredentialView* ptr = dynamic_cast<CredentialView*>(widget(i));
            if (ptr && ptr->GetID() == credential_id)
            {
                ptr->UpdateTable(platform_id);
                ptr->UpdateInfo();
                bResult2 = true;
            }
        }

        if (bResult1 && bResult2) return true;
    }

    return false;
}

bool StackView::UpdateAccount(unsigned int credential_id, unsigned int platform_id, unsigned int account_id)
{
    bool bResult1 = false, bResult2 = false, bResult3 = false;

    for (int i = 0; i < count(); ++i)
    {
        if (!bResult1)
        {
            AccountView* ptr = dynamic_cast<AccountView*>(widget(i));
            if (ptr && ptr->GetID() == account_id)
            {
                ptr->UpdateInfo();
                bResult1 = true;
            }
        }
        if (!bResult2)
        {
            PlatformView* ptr = dynamic_cast<PlatformView*>(widget(i));
            if (ptr && ptr->GetID() == platform_id)
            {
                ptr->UpdateTable(account_id);
                bResult2 = true;
            }
        }
        if (!bResult3)
        {
            CredentialView* ptr = dynamic_cast<CredentialView*>(widget(i));
            if (ptr && ptr->GetID() == credential_id)
            {
                ptr->UpdateInfo();
                bResult3 = true;
            }
        }

        if (bResult1 && bResult2 && bResult3) return true;
    }

    return false;
}

bool StackView::UpdateProperty(unsigned int credential_id, unsigned int account_id, unsigned int property_id)
{
    bool bResult1 = false, bResult2 = false;

    for (int i = 0; i < count(); ++i)
    {
        if (!bResult1)
        {
            AccountView* ptr = dynamic_cast<AccountView*>(widget(i));
            if (ptr && ptr->GetID() == account_id)
            {
                ptr->UpdateTable(property_id);
                bResult1 = true;
            }
        }
        if (!bResult2)
        {
            CredentialView* ptr = dynamic_cast<CredentialView*>(widget(i));
            if (ptr && ptr->GetID() == credential_id)
            {
                ptr->UpdateInfo();
                bResult2 = true;
            }
        }

        if (bResult1 && bResult2) return true;
    }

    return false;
}

bool StackView::RemovePlatform(unsigned int credential_id, unsigned int platform_id, const std::vector<unsigned int>& ids)
{
    RemoveView(ids);

    for (int i = 0; i < count(); ++i)
    {
        CredentialView* ptr = dynamic_cast<CredentialView*>(widget(i));
        if (ptr && ptr->GetID() == credential_id)
        {
            ptr->UpdateTable();
            ptr->UpdateInfo();
            return true;
        }
    }

    return false;
}

bool StackView::RemoveAccount(unsigned int credential_id, unsigned int platform_id, unsigned int account_id, const std::vector<unsigned int>& ids)
{
    RemoveView(ids);

    return UpdateView<PlatformView>(credential_id, platform_id);
}

bool StackView::RemoveProperty(unsigned int credential_id, unsigned int account_id, unsigned int property_id)
{
    return UpdateView<AccountView>(credential_id, account_id);
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

bool StackView::SwitchToView(bnb::credential_enum eType, unsigned int id)
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
