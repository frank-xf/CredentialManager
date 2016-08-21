#include <list>

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>

#include "../CredentialManager/bnb_global.h"
#include "../CredentialManager/Credential/Credential.h"

#include "CredentialView.h"

CredentialView::CredentialView(QWidget * parent, const bnb::Credential& src)
    : QWidget(parent)
{
    MakeLabelList(src);
    LayoutView();
}

void CredentialView::MakeLabelList(const bnb::Credential & src)
{
    for (auto ptr_platform = src->Head(); ptr_platform; ptr_platform = ptr_platform->m_Next)
    {
        _ui_element item;
        item.m_labPlatform = new QLabel(this);
        item.m_labPlatform->setText(QString::fromStdString(ptr_platform->m_Pair.m_Key.m_Key));

        for (auto ptr_account = ptr_platform->m_Pair.m_Value.Head(); ptr_account; ptr_account = ptr_account->m_Next)
        {
            QLabel* labAccount = new QLabel(this);
            labAccount->setText(QString::fromStdString(ptr_account->m_Pair.m_Key.m_Key));

            item.m_listAccount.push_back(labAccount);
        }

        m_listLabel.push_back(item);
    }
}

void CredentialView::LayoutView()
{
    QVBoxLayout* pMainLayout = new QVBoxLayout;

    if (m_listLabel.empty())
    {
        QLabel* labHint = new QLabel(this);
        labHint->setText(u8R"(You haven't added any credential yet)");
        pMainLayout->addWidget(labHint);

        return;
    }

    for (const auto& item : m_listLabel)
    {
        QHBoxLayout* phLayout = new QHBoxLayout;
        phLayout->addWidget(item.m_labPlatform);

        if (!item.m_listAccount.empty())
        {
            QVBoxLayout* pvLayout = new QVBoxLayout;
            for (const auto& iter : item.m_listAccount)
                pvLayout->addWidget(iter);

            phLayout->addLayout(pvLayout);
        }

        pMainLayout->addLayout(phLayout);
    }

    setLayout(pMainLayout);
}
