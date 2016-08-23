#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenu>
#include <QtGui/QContextMenuEvent>

#include "../CredentialManager/bnb_global.h"
#include "../CredentialManager/Credential/Credential.h"

#include "CredentialItem.h"

QT_BEGIN_NAMESPACE

//==============================================================================
// Implementation of AccountLabel
//==============================================================================
AccountLabel::AccountLabel(bnb::account_type* pAccount, delegate_type* ptrDelegate, QWidget * parent)
    : QWidget(parent)
    , m_ptrDelegate(ptrDelegate)
    , m_ptrAccount(pAccount)
{
    _ui.SetupUI(this);

    Update();

    setFixedSize(240, 28);

    QObject::connect(_ui.m_actView, &QAction::triggered, this, &AccountLabel::OnClickedView);
    QObject::connect(_ui.m_actEdit, &QAction::triggered, this, &AccountLabel::OnClickedEdit);
    QObject::connect(_ui.m_actRemove, &QAction::triggered, this, &AccountLabel::OnClickedRemove);
}

void AccountLabel::Update()
{
    _ui.m_labAccount->setText(QString::fromStdString(m_ptrAccount->m_Key));
}

void AccountLabel::contextMenuEvent(QContextMenuEvent * event)
{
    QMenu menu(this);

    menu.addAction(_ui.m_actView);
    menu.addSeparator();
    menu.addAction(_ui.m_actEdit);
    menu.addAction(_ui.m_actRemove);

    menu.exec(event->globalPos());
}

void AccountLabel::mouseDoubleClickEvent(QMouseEvent * event)
{
    OnClickedView();
}

void AccountLabel::OnClickedView()
{
    m_ptrDelegate->OnViewCredential(m_ptrAccount);
}

void AccountLabel::OnClickedEdit()
{
    m_ptrDelegate->OnEditAccount(m_ptrAccount);
    Update();
}

void AccountLabel::OnClickedRemove()
{
    m_ptrDelegate->OnRemoveAccount(m_ptrAccount);
}

//------------------------------------------------------------------------------

void AccountLabel::ui_type::SetupUI(AccountLabel * pView)
{
    m_actView = new QAction(pView);
    m_actEdit = new QAction(pView);
    m_actRemove = new QAction(pView);

    m_labAccount = new QLabel(pView);
    m_labAccount->setAlignment(Qt::AlignCenter);
    m_labAccount->setStyleSheet("QLabel{ background-color:#80FF80; }");

    QVBoxLayout* pLayout = new QVBoxLayout;
    pLayout->setSpacing(0);
    pLayout->setMargin(0);
    pLayout->addWidget(m_labAccount);

    pView->setLayout(pLayout);

    RetranslateUI(pView);
}

void AccountLabel::ui_type::RetranslateUI(AccountLabel * pView)
{
    m_actView->setText("View Credential");
    m_actEdit->setText("Edit Account");
    m_actRemove->setText("Remove Account");
}

//==============================================================================
// Implementation of PlatformLabel
//==============================================================================
PlatformLabel::PlatformLabel(bnb::platform_type* ptrPlatform, delegate_type* ptrDelegate, QWidget * parent)
    : QWidget(parent)
    , m_ptrDelegate(ptrDelegate)
    , m_ptrPlatform(ptrPlatform)
{
    _ui.SetupUI(this);

    Update();

    setMinimumSize(400, 28);

    QObject::connect(_ui.m_actView, &QAction::triggered, this, &PlatformLabel::OnClickedView);
    QObject::connect(_ui.m_actEdit, &QAction::triggered, this, &PlatformLabel::OnClickedEdit);
    QObject::connect(_ui.m_actRemove, &QAction::triggered, this, &PlatformLabel::OnClickedRemove);
    QObject::connect(_ui.m_actAddPlatform, &QAction::triggered, this, &PlatformLabel::OnClickedAddPlatform);
    QObject::connect(_ui.m_actAddAccount, &QAction::triggered, this, &PlatformLabel::OnClickedAddAccount);
}

void PlatformLabel::Update()
{
    _ui.m_labPlatform->setText(QString::fromStdString(m_ptrPlatform->m_Key));
}

void PlatformLabel::contextMenuEvent(QContextMenuEvent * event)
{
    QMenu menu(this);

    menu.addAction(_ui.m_actView);
    menu.addSeparator();
    menu.addAction(_ui.m_actEdit);
    menu.addAction(_ui.m_actRemove);
    menu.addSeparator();
    menu.addAction(_ui.m_actAddPlatform);
    menu.addAction(_ui.m_actAddAccount);

    menu.exec(event->globalPos());
}

void PlatformLabel::mouseDoubleClickEvent(QMouseEvent * event)
{
    OnClickedView();
}

void PlatformLabel::OnClickedView()
{
    m_ptrDelegate->OnViewCredential(m_ptrPlatform);
}

void PlatformLabel::OnClickedEdit()
{
    m_ptrDelegate->OnEditPlatform(m_ptrPlatform);
    Update();
}

void PlatformLabel::OnClickedRemove()
{
    m_ptrDelegate->OnRemovePlatform(m_ptrPlatform);
}

void PlatformLabel::OnClickedAddPlatform()
{
    m_ptrDelegate->OnAddPlatform();
}

void PlatformLabel::OnClickedAddAccount()
{
    m_ptrDelegate->OnAddAccount();
}

//------------------------------------------------------------------------------

void PlatformLabel::ui_type::SetupUI(PlatformLabel * pView)
{
    m_actView = new QAction(pView);
    m_actEdit = new QAction(pView);
    m_actRemove = new QAction(pView);
    m_actAddPlatform = new QAction(pView);
    m_actAddAccount = new QAction(pView);

    m_labPlatform = new QLabel(pView);
    m_labPlatform->setAlignment(Qt::AlignCenter);
    m_labPlatform->setStyleSheet("QLabel{ background-color:#80FF80; }");

    QVBoxLayout* pLayout = new QVBoxLayout;
    pLayout->setSpacing(0);
    pLayout->setMargin(0);
    pLayout->addWidget(m_labPlatform);

    pView->setLayout(pLayout);

    RetranslateUI(pView);
}

void PlatformLabel::ui_type::RetranslateUI(PlatformLabel * pView)
{
    m_actView->setText("View Credential");
    m_actEdit->setText("Edit Platform");
    m_actRemove->setText("Remove Platform");
    m_actAddPlatform->setText("Add Platform");
    m_actAddAccount->setText("Add Account");
}

//==============================================================================
// Implementation of CredentialItem
//==============================================================================
CredentialItem::CredentialItem(bnb::platform_type& platform, bnb::account_list& account, delegate_type* ptrDelegate, QWidget * parent)
    : QWidget(parent)
    , m_ptrDelegate(ptrDelegate)
{
    m_labPlatform = new PlatformLabel(&platform, this, this);

    QHBoxLayout* phLayout = new QHBoxLayout;
    phLayout->setMargin(0);
    phLayout->setSpacing(1);
    phLayout->addWidget(m_labPlatform);

    if (!account.IsEmpty())
    {
        QVBoxLayout* pvLayout = new QVBoxLayout;
        pvLayout->setMargin(0);
        pvLayout->setSpacing(1);

        for (auto ptr_account = account.Head(); ptr_account; ptr_account = ptr_account->m_Next)
            pvLayout->addWidget(new AccountLabel(&ptr_account->m_Pair.m_Key, this, this));

        phLayout->addLayout(pvLayout);
    }

    setLayout(phLayout);
}

bool CredentialItem::OnAddPlatform()
{
    return m_ptrDelegate->OnAddPlatform();
}

bool CredentialItem::OnAddAccount()
{
    return m_ptrDelegate->OnAddAccount();
}

bool CredentialItem::OnEditPlatform(bnb::platform_type * ptrPlatform)
{
    return m_ptrDelegate->OnEditPlatform(ptrPlatform);
}

bool CredentialItem::OnEditAccount(bnb::account_type * pAccount)
{
    return m_ptrDelegate->OnEditAccount(m_labPlatform->GetPlatform(), pAccount);
}

bool CredentialItem::OnRemoveAccount(bnb::account_type * pAccount)
{
    return m_ptrDelegate->OnRemoveAccount(m_labPlatform->GetPlatform(), pAccount);
}

bool CredentialItem::OnRemovePlatform(bnb::platform_type * ptrPlatform)
{
    return m_ptrDelegate->OnRemovePlatform(ptrPlatform);
}

bool CredentialItem::OnViewCredential(bnb::platform_type * ptrPlatform)
{
    return m_ptrDelegate->OnViewCredential(ptrPlatform, nullptr);
}

bool CredentialItem::OnViewCredential(bnb::account_type * pAccount)
{
    return m_ptrDelegate->OnViewCredential(m_labPlatform->GetPlatform(), pAccount);
}

QT_END_NAMESPACE
