#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QStackedWidget>

#include "Credential/Credential.h"

#include "credential_qt_delegate.h"
#include "credential_qt_utils.h"

#include "Widget/DropTable.h"
#include "Widget/ContentView.h"
#include "Major/StackView.h"

QT_BEGIN_NAMESPACE

StackView::StackView(DelegateType* pDelegate, QWidget * parent)
    : QStackedWidget(parent)
    , _delegate(pDelegate)
{
    setObjectName("StackView");
    ui_utils::SetBackgroundColor(this, Qt::white);

    m_labHint = new QLabel("Please open or new a credential file.", this);
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
    auto view_credential = new CredentialView(credential, _delegate, this);
    addWidget(view_credential);

    credential.PreorderTraversal([this](const bnb::platform_node& platform) {
        addWidget(new PlatformView(platform, _delegate, this));

        platform.PreorderTraversal([this](const bnb::account_node& account) {
            addWidget(new AccountView(account, _delegate, this));
        });
    });

    setCurrentWidget(view_credential);
}

bool StackView::AddPlatform(const bnb::platform_node & platform)
{
    if (auto ptr_credential = dynamic_cast<bnb::Credential*>(platform.GetParent()))
    {
        addWidget(new PlatformView(platform, _delegate, this));
        UpdateTable(ptr_credential->GetID());
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
            addWidget(new AccountView(account, _delegate, this));
            UpdateTable(ptr_credential->GetID(), ptr_platform->GetID());
            return true;
        }
    }

    return false;
}

bool StackView::AddPair(const bnb::pair_node & pair)
{
    if (auto ptr_account = dynamic_cast<bnb::account_node*>(pair.GetParent()))
        if (auto ptr_platform = dynamic_cast<bnb::platform_node*>(ptr_account->GetParent()))
            if (auto ptr_credential = dynamic_cast<bnb::Credential*>(ptr_platform->GetParent()))
                return UpdateTable(ptr_credential->GetID(), ptr_platform->GetID(), ptr_account->GetID());

    return false;
}

bool StackView::UpdateCredential(id_type credentialId)
{
    for (int i = 0; i < count(); ++i)
    {
        CredentialView* ptr = dynamic_cast<CredentialView*>(widget(i));
        if (ptr && ptr->GetID() == credentialId)
        {
            ptr->UpdateInfo();
            return true;
        }
    }

    return false;
}

bool StackView::UpdatePlatform(id_type credentialId, id_type platformId)
{
    bool bResult1 = false, bResult2 = false;

    for (int i = 0; i < count(); ++i)
    {
        if (!bResult1)
        {
            PlatformView* ptr = dynamic_cast<PlatformView*>(widget(i));
            if (ptr && ptr->GetID() == platformId)
            {
                ptr->UpdateInfo();
                bResult1 = true;
            }
        }
        if (!bResult2)
        {
            CredentialView* ptr = dynamic_cast<CredentialView*>(widget(i));
            if (ptr && ptr->GetID() == credentialId)
            {
                ptr->UpdateTable(platformId);
                ptr->UpdateInfo();
                bResult2 = true;
            }
        }

        if (bResult1 && bResult2) return true;
    }

    return false;
}

bool StackView::UpdateAccount(id_type credentialId, id_type platformId, id_type accountId)
{
    bool bResult1 = false, bResult2 = false, bResult3 = false;

    for (int i = 0; i < count(); ++i)
    {
        if (!bResult1)
        {
            AccountView* ptr = dynamic_cast<AccountView*>(widget(i));
            if (ptr && ptr->GetID() == accountId)
            {
                ptr->UpdateInfo();
                bResult1 = true;
            }
        }
        if (!bResult2)
        {
            PlatformView* ptr = dynamic_cast<PlatformView*>(widget(i));
            if (ptr && ptr->GetID() == platformId)
            {
                ptr->UpdateTable(accountId);
                bResult2 = true;
            }
        }
        if (!bResult3)
        {
            CredentialView* ptr = dynamic_cast<CredentialView*>(widget(i));
            if (ptr && ptr->GetID() == credentialId)
            {
                ptr->UpdateInfo();
                bResult3 = true;
            }
        }

        if (bResult1 && bResult2 && bResult3) return true;
    }

    return false;
}

bool StackView::UpdatePair(id_type credentialId, id_type accountId, id_type pairId)
{
    bool bResult1 = false, bResult2 = false;

    for (int i = 0; i < count(); ++i)
    {
        if (!bResult1)
        {
            AccountView* ptr = dynamic_cast<AccountView*>(widget(i));
            if (ptr && ptr->GetID() == accountId)
            {
                ptr->UpdateTable(pairId);
                bResult1 = true;
            }
        }
        if (!bResult2)
        {
            CredentialView* ptr = dynamic_cast<CredentialView*>(widget(i));
            if (ptr && ptr->GetID() == credentialId)
            {
                ptr->UpdateInfo();
                bResult2 = true;
            }
        }

        if (bResult1 && bResult2) return true;
    }

    return false;
}

bool StackView::UpdateTable(id_type credentialId)
{
    for (int i = 0; i < count(); ++i)
    {
        CredentialView* ptr = dynamic_cast<CredentialView*>(widget(i));
        if (ptr && ptr->GetID() == credentialId)
        {
            ptr->UpdateTable();
            ptr->UpdateInfo();
            return true;
        }
    }

    return false;
}

bool StackView::UpdateTable(id_type credentialId, id_type platformId)
{
    return UpdateView<PlatformView>(credentialId, platformId);
}

bool StackView::UpdateTable(id_type credentialId, id_type platformId, id_type accountId)
{
    return UpdateView<AccountView>(credentialId, accountId);
}

bool StackView::RemoveCredential(id_type credentialId, const std::vector<id_type>& ids)
{
    RemoveView(ids);

    return false;
}

bool StackView::RemovePlatform(id_type credentialId, id_type platformId, const std::vector<id_type>& ids)
{
    RemoveView(ids);

    return UpdateTable(credentialId);
}

bool StackView::RemoveAccount(id_type credentialId, id_type platformId, id_type accountId, const std::vector<id_type>& ids)
{
    RemoveView(ids);

    return UpdateTable(credentialId, platformId);
}

bool StackView::RemovePair(id_type credentialId, id_type platformId, id_type accountId, id_type pairId)
{
    return UpdateTable(credentialId, platformId, accountId);
}

unsigned int StackView::RemoveView(const std::vector<id_type>& ids)
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

bool StackView::SwitchToView(bnb::credential_enum eType, id_type id)
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

void StackView::SwitchToHint()
{
    setCurrentWidget(m_labHint);
}

QT_END_NAMESPACE
