#include <QtCore/QDateTime>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>

#include "../../cm-core/cm_core.h"

#include "../cm_view_delegate.h"
#include "../cm_view_utils.h"

#include "DropTable.h"
#include "NodeView.h"

QT_BEGIN_NAMESPACE

inline QTableWidgetItem* MakeTableItem(const QString& strText, const QColor& color, const void* data)
{
    QTableWidgetItem* pItem = new QTableWidgetItem(strText);
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setTextColor(color);
    pItem->setData(Qt::UserRole, QVariant::fromValue(data));
    pItem->setFont(MakeFont());

    return pItem;
}

NodeView::NodeView(QWidget* parent)
    : QWidget(parent)
{
    _ui.SetupUI(this);

    QObject::connect(_ui.m_btnEdit, &QPushButton::clicked, [this]() { OnClickedEdit(); });
    QObject::connect(_ui.m_btnRemove, &QPushButton::clicked, [this]() { OnClickedRemove(); });
}

void NodeView::Show(const xf::credential::credential_t& credential)
{
    _ui.AdjustIndicator(0);
    _ui.AdjustLabel(3);

    _ui.m_Displays[0].first->setText("Update Time: ");
    _ui.m_Displays[1].first->setText("User Name: ");
    _ui.m_Displays[2].first->setText("Description: ");
    _ui.m_Displays[0].second->setText(QDateTime::fromTime_t(credential.Time()).toString("yyyy-MM-dd HH:mm:ss"));
    _ui.m_Displays[1].second->setText(ToQString(credential.Username()));
    _ui.m_Displays[2].second->setText(ToQString(credential.Description()));

    _ui.m_tabView->clearContents();

    unsigned int nRows = credential.Size();

    if (0 < nRows)
    {
        _ui.m_tabView->setRowCount(nRows);
        _ui.m_tabView->setColumnCount(3);
        _ui.m_tabView->setHorizontalHeaderLabels({ "    Platform    ", "        Url        ", "        Description        " });

        unsigned int nIndex = 0;
        credential.Traversal([this, &nIndex](const xf::credential::platform_t& platform) mutable {
            auto pName = MakeTableItem(ToQString(platform.Item().name), { 255, 64, 0 }, &platform);
            auto pUrl = MakeTableItem(ToQString(platform.Item().url), { 64, 64, 255 }, &platform);
            auto pDisplay = MakeTableItem(ToQString(platform.Item().description), { 32, 160, 32 }, &platform);

            _ui.m_tabView->setItem(nIndex, 0, pName);
            _ui.m_tabView->setItem(nIndex, 1, pUrl);
            _ui.m_tabView->setItem(nIndex, 2, pDisplay);

            ++nIndex;
        });
    }
}

void NodeView::ui_type::AdjustIndicator(unsigned int nIndicator)
{
    for (unsigned int i = 0; i < _IndicatorSize; ++i)
    {
        if (i < nIndicator)
        {
            m_Indicators[i].first->show();
            m_Indicators[i].second->show();
        }
        else
        {
            m_Indicators[i].first->hide();
            m_Indicators[i].second->hide();
        }
    }
}

void NodeView::ui_type::AdjustLabel(unsigned int nDisplay)
{
    for (unsigned int i = 0; i < _FieldMaxSize; ++i)
    {
        if (i < nDisplay)
        {
            m_Displays[i].first->show();
            m_Displays[i].second->show();
        }
        else
        {
            m_Displays[i].first->hide();
            m_Displays[i].second->hide();
        }
    }
}

void NodeView::ui_type::SetupUI(NodeView* pView)
{
    for (unsigned int i = 0; i < _IndicatorSize; ++i)
    {
        m_Indicators[i].first = MakeStaticLabel(pView, g_clrLabel);
        m_Indicators[i].second = MakeTransparentButton(pView);
    }

    for (unsigned int i = 0; i < _FieldMaxSize; ++i)
    {
        m_Displays[i].first = MakeStaticLabel(pView, g_clrLabel);
        m_Displays[i].second = MakeShowLine(pView);
    }

    m_btnEdit = MakeTransparentButton(pView);
    m_btnRemove = MakeTransparentButton(pView);

    m_tabView = new DropTable(pView, pView);

    QHBoxLayout* layoutIndicator = new QHBoxLayout;
    layoutIndicator->setMargin(0);
    layoutIndicator->setSpacing(8);
    for (unsigned int i = 0; i < _IndicatorSize; ++i)
    {
        layoutIndicator->addWidget(m_Indicators[i].first);
        layoutIndicator->addWidget(m_Indicators[i].second);
    }
    layoutIndicator->addStretch(1);
    layoutIndicator->addWidget(m_btnEdit);
    layoutIndicator->addWidget(m_btnRemove);

    QVBoxLayout* layoutLabelLeft = new QVBoxLayout;
    QVBoxLayout* layoutEditRight = new QVBoxLayout;
    layoutLabelLeft->setMargin(0);
    layoutEditRight->setMargin(0);
    layoutLabelLeft->setSpacing(2);
    layoutEditRight->setSpacing(2);
    for (unsigned int i = 0; i < _FieldMaxSize; ++i)
    {
        layoutLabelLeft->addWidget(m_Displays[i].first);
        layoutEditRight->addWidget(m_Displays[i].second);
    }

    QHBoxLayout* phLayout = new QHBoxLayout;
    phLayout->setContentsMargins(0, 0, 0, 16);
    phLayout->setSpacing(0);
    phLayout->addLayout(layoutLabelLeft);
    phLayout->addLayout(layoutEditRight);

    QVBoxLayout* pvMainLayout = new QVBoxLayout;
    pvMainLayout->setMargin(0);
    pvMainLayout->setSpacing(0);
    pvMainLayout->addLayout(layoutIndicator);
    pvMainLayout->addLayout(phLayout);
    pvMainLayout->addWidget(m_tabView, 1);

    QHBoxLayout* phMainLayout = new QHBoxLayout;
    phMainLayout->setContentsMargins(4, 0, 4, 4);
    phMainLayout->setSpacing(0);
    phMainLayout->addStretch(1);
    phMainLayout->addLayout(pvMainLayout, 8);
    phMainLayout->addStretch(1);

    pView->setLayout(phMainLayout);

    m_btnEdit->setText("Edit");
    m_btnRemove->setText("Remove");

    RetranslateUI(pView);
}

void NodeView::ui_type::RetranslateUI(NodeView* pView) { }

//==============================================================================
// Implementation of CredentialView
//==============================================================================
CredentialView::CredentialView(const bnb::Credential& credential, DelegateType* pDelegate, QWidget * parent)
    : base_type(credential.GetID(), pDelegate, parent)
    , m_Credential(credential)
{
    UpdateInfo();
    UpdateTable();
}

void CredentialView::UpdateInfo()
{
    _ui.m_editText[0]->setText(QDateTime::fromTime_t(m_Credential.GetData().GetTime()).toString("yyyy-MM-dd HH:mm:ss"));
    _ui.m_editText[1]->setText(To_QString(m_Credential.GetData().GetUser()));
    _ui.m_editText[2]->setText(To_QString(m_Credential.GetData().GetComment()));
}

void CredentialView::UpdateTable()
{
    _ui.m_tabView->clearContents();

    unsigned int nRows = m_Credential.Size();
    _ui.m_tabView->setRowCount(nRows);

    if (0 < nRows)
    {
        unsigned int nIndex = 0;
        m_Credential.PreorderTraversal([this, &nIndex](const bnb::platform_node& platform) mutable {
            auto pName = MakeTableItem(To_QString(platform.GetData().GetName()), platform.GetID(), { 255, 64, 0 }, Qt::AlignCenter);
            auto pUrl = MakeTableItem(To_QString(platform.GetData().GetUrl()), platform.GetID(), { 64, 64, 255 });
            auto pDisplay = MakeTableItem(To_QString(platform.GetData().GetComment()), platform.GetID(), { 32, 160, 32 });

            _ui.m_tabView->setItem(nIndex, 0, pName);
            _ui.m_tabView->setItem(nIndex, 1, pUrl);
            _ui.m_tabView->setItem(nIndex, 2, pDisplay);

            ++nIndex;
        });
    }
}

void CredentialView::UpdateTable(id_type id)
{
    for (int i = 0; i < _ui.m_tabView->rowCount(); ++i)
    {
        if (auto pItem = _ui.m_tabView->item(i, 0))
        {
            if (id == pItem->data(Qt::UserRole).toUInt())
            {
                m_Credential.PreorderAction([this, pItem, id, i](const bnb::platform_node& platform) {
                    if (platform.GetID() == id)
                    {
                        pItem->setText(To_QString(platform.GetData().GetName()));
                        _ui.m_tabView->item(i, 1)->setText(To_QString(platform.GetData().GetUrl()));
                        _ui.m_tabView->item(i, 2)->setText(To_QString(platform.GetData().GetComment()));
                        return true;
                    }

                    return false;
                });

                break;
            }
        }
    }
}

void CredentialView::OnAdd()
{
    if (_delegate)
        _delegate->OnAddPlatform(m_Credential.GetID());
}

void CredentialView::OnEdit(unsigned int id)
{
    if (_delegate)
        _delegate->OnUpdatePlatform(m_Credential.GetID(), id);
}

void CredentialView::OnRemove(unsigned int id)
{
    if (_delegate)
        _delegate->OnRemovePlatform(m_Credential.GetID(), id);
}

void CredentialView::OnMove(unsigned int id, int offset)
{
    if (_delegate)
        _delegate->OnMovePlatform(m_Credential.GetID(), id, offset);
}

void CredentialView::OnSort(int cln, bool ascending)
{
    if (_delegate)
        _delegate->OnSortPlatform(m_Credential.GetID(), cln, ascending);
}

void CredentialView::OnDoubleClicked(unsigned int id)
{
    if (_delegate)
        _delegate->SwitchToNode(m_Credential.GetID(), id);
}

void CredentialView::OnClickedEdit()
{
    if (_delegate)
        _delegate->OnUpdateCredential(m_Credential.GetID());
}

void CredentialView::OnClickedRemove()
{
    if (_delegate)
        _delegate->OnRemoveCredential(m_Credential.GetID());
}

//------------------------------------------------------------------------------

template<>
void CredentialView::base_type::ui_type::RetranslateUI(CredentialView::base_type* pView)
{
    m_btnBack->hide();
    m_tabView->setColumnCount(3);
    m_tabView->setHorizontalHeaderLabels({ "    Platform    ", "        Url        ", "        Description        " });

    _labText[0]->setText("Update Time: ");
    _labText[1]->setText("User Name: ");
    _labText[2]->setText("Description: ");
}

template<>
void CredentialView::base_type::ui_type::CreateLabel(CredentialView::base_type* pView)
{
    for (unsigned int i = 0; i < 3; ++i)
    {
        _labText[i] = MakeStaticLabel(pView);
        m_editText[i] = MakeShowLine(pView, g_clrCredential);
    }
}

//==============================================================================
// Implementation of PlatformView
//==============================================================================
PlatformView::PlatformView(const bnb::platform_node& tp, DelegateType* pDelegate, QWidget * parent)
    : base_type(tp.GetID(), pDelegate, parent)
    , m_Platform(tp)
{
    UpdateInfo();
    UpdateTable();
}

void PlatformView::UpdateInfo()
{
    _ui.m_editText[0]->setText(To_QString(m_Platform.GetData().GetName()));
    _ui.m_editText[1]->setText(To_QString(m_Platform.GetData().GetUrl()));
    _ui.m_editText[2]->setText(To_QString(m_Platform.GetData().GetComment()));
}

void PlatformView::UpdateTable()
{
    _ui.m_tabView->clearContents();

    unsigned int nRows = m_Platform.Size();
    _ui.m_tabView->setRowCount(nRows);

    if (0 < nRows)
    {
        unsigned int nIndex = 0;
        m_Platform.PreorderTraversal([this, &nIndex](const bnb::account_node& account) mutable {
            auto pName = MakeTableItem(To_QString(account.GetData().GetName()), account.GetID(), { 64, 64, 255 }, Qt::AlignCenter);
            auto pDisplay = MakeTableItem(To_QString(account.GetData().GetComment()), account.GetID(), { 32, 160, 32 });

            _ui.m_tabView->setItem(nIndex, 0, pName);
            _ui.m_tabView->setItem(nIndex, 1, pDisplay);

            ++nIndex;
        });
    }
}

void PlatformView::UpdateTable(id_type id)
{
    for (int i = 0; i < _ui.m_tabView->rowCount(); ++i)
    {
        if (auto pItem = _ui.m_tabView->item(i, 0))
        {
            if (id == pItem->data(Qt::UserRole).toUInt())
            {
                m_Platform.PreorderAction([this, pItem, id, i](const bnb::account_node& account) {
                    if (account.GetID() == id)
                    {
                        pItem->setText(To_QString(account.GetData().GetName()));
                        _ui.m_tabView->item(i, 1)->setText(To_QString(account.GetData().GetComment()));
                        return true;
                    }

                    return false;
                });

                break;
            }
        }
    }
}

void PlatformView::OnAdd()
{
    if (auto ptr_credential = dynamic_cast<bnb::Credential*>(m_Platform.GetParent()))
        if (_delegate)
            _delegate->OnAddAccount(ptr_credential->GetID(), m_Platform.GetID());
}

void PlatformView::OnEdit(unsigned int id)
{
    if (auto ptr_credential = dynamic_cast<bnb::Credential*>(m_Platform.GetParent()))
        if (_delegate)
            _delegate->OnUpdateAccount(ptr_credential->GetID(), m_Platform.GetID(), id);
}

void PlatformView::OnRemove(unsigned int id)
{
    if (auto ptr_credential = dynamic_cast<bnb::Credential*>(m_Platform.GetParent()))
        if (_delegate)
            _delegate->OnRemoveAccount(ptr_credential->GetID(), m_Platform.GetID(), id);
}

void PlatformView::OnMove(unsigned int id, int offset)
{
    if (auto ptr_credential = dynamic_cast<bnb::Credential*>(m_Platform.GetParent()))
        if (_delegate)
            _delegate->OnMoveAccount(ptr_credential->GetID(), m_Platform.GetID(), id, offset);
}

void PlatformView::OnSort(int cln, bool ascending)
{
    if (auto ptr_credential = dynamic_cast<bnb::Credential*>(m_Platform.GetParent()))
        if (_delegate)
            _delegate->OnSortAccount(ptr_credential->GetID(), m_Platform.GetID(), cln, ascending);
}

void PlatformView::OnDoubleClicked(unsigned int id)
{
    if (auto ptr_credential = dynamic_cast<bnb::Credential*>(m_Platform.GetParent()))
        if (_delegate)
            _delegate->SwitchToNode(ptr_credential->GetID(), m_Platform.GetID(), id);
}

void PlatformView::OnClickedBack()
{
    if (auto ptr_credential = dynamic_cast<bnb::Credential*>(m_Platform.GetParent()))
        if (_delegate)
            _delegate->SwitchToNode(ptr_credential->GetID());
}

void PlatformView::OnClickedEdit()
{
    if (auto ptr_credential = dynamic_cast<bnb::Credential*>(m_Platform.GetParent()))
        if (_delegate)
            _delegate->OnUpdatePlatform(ptr_credential->GetID(), m_Platform.GetID());
}

void PlatformView::OnClickedRemove()
{
    if (auto ptr_credential = dynamic_cast<bnb::Credential*>(m_Platform.GetParent()))
        if (_delegate)
            _delegate->OnRemovePlatform(ptr_credential->GetID(), m_Platform.GetID());
}

//------------------------------------------------------------------------------

template<>
void PlatformView::base_type::ui_type::RetranslateUI(PlatformView::base_type* pView)
{
    m_tabView->setColumnCount(2);
    m_tabView->setHorizontalHeaderLabels({ "        Account        ", "        Description        " });

    _labText[0]->setText("Platform: ");
    _labText[1]->setText("Url: ");
    _labText[2]->setText("Description: ");
}

template<>
void PlatformView::base_type::ui_type::CreateLabel(PlatformView::base_type* pView)
{
    for (unsigned int i = 0; i < 3; ++i)
    {
        _labText[i] = ui_utils::MakeStaticLabel(pView);
        m_editText[i] = ui_utils::MakeShowLine(pView, ui_utils::g_clrPlatform);
    }
}

//==============================================================================
// Implementation of AccountView
//==============================================================================
AccountView::AccountView(const bnb::account_node & tp, DelegateType* pDelegate, QWidget * parent)
    : base_type(tp.GetID(), pDelegate, parent)
    , m_Account(tp)
{
    UpdateInfo();
    UpdateTable();
}

void AccountView::UpdateInfo()
{
    _ui.m_editText[0]->setText(To_QString(m_Account.GetData().GetName()));
    _ui.m_editText[1]->setText(To_QString(m_Account.GetData().GetComment()));
}

void AccountView::UpdateTable()
{
    _ui.m_tabView->clearContents();

    unsigned int nRows = m_Account.Size();
    _ui.m_tabView->setRowCount(nRows);

    if (0 < nRows)
    {
        unsigned int nIndex = 0;
        m_Account.PreorderTraversal([this, &nIndex](const bnb::pair_node& pair) mutable {
            auto pKey = MakeTableItem(To_QString(pair.GetData().GetKey()), pair.GetID(), { 32, 160, 32 }, Qt::AlignCenter);
            auto pValue = MakeTableItem(To_QString(pair.GetData().GetValue()), pair.GetID(), { 32, 160, 32 }, Qt::AlignCenter);

            _ui.m_tabView->setItem(nIndex, 0, pKey);
            _ui.m_tabView->setItem(nIndex, 1, pValue);

            ++nIndex;
        });
    }
}

void AccountView::UpdateTable(id_type id)
{
    for (int i = 0; i < _ui.m_tabView->rowCount(); ++i)
    {
        if (auto pItem = _ui.m_tabView->item(i, 0))
        {
            if (id == pItem->data(Qt::UserRole).toUInt())
            {
                m_Account.PreorderAction([this, pItem, id, i](const bnb::pair_node& pair) {
                    if (pair.GetID() == id)
                    {
                        pItem->setText(To_QString(pair.GetData().GetKey()));
                        _ui.m_tabView->item(i, 1)->setText(To_QString(pair.GetData().GetValue()));
                        return true;
                    }

                    return false;
                });

                break;
            }
        }
    }
}

void AccountView::OnAdd()
{
    if (auto ptr_platform = dynamic_cast<bnb::platform_node*>(m_Account.GetParent()))
        if (auto ptr_credential = dynamic_cast<bnb::Credential*>(ptr_platform->GetParent()))
            if (_delegate)
                _delegate->OnAddPair(ptr_credential->GetID(), ptr_platform->GetID(), m_Account.GetID());
}

void AccountView::OnEdit(unsigned int id)
{
    if (auto ptr_platform = dynamic_cast<bnb::platform_node*>(m_Account.GetParent()))
        if (auto ptr_credential = dynamic_cast<bnb::Credential*>(ptr_platform->GetParent()))
            if (_delegate)
                _delegate->OnUpdatePair(ptr_credential->GetID(), ptr_platform->GetID(), m_Account.GetID(), id);
}

void AccountView::OnRemove(unsigned int id)
{
    if (auto ptr_platform = dynamic_cast<bnb::platform_node*>(m_Account.GetParent()))
        if (auto ptr_credential = dynamic_cast<bnb::Credential*>(ptr_platform->GetParent()))
            if (_delegate)
                _delegate->OnRemovePair(ptr_credential->GetID(), ptr_platform->GetID(), m_Account.GetID(), id);
}

void AccountView::OnMove(unsigned int id, int offset)
{
    if (auto ptr_platform = dynamic_cast<bnb::platform_node*>(m_Account.GetParent()))
        if (auto ptr_credential = dynamic_cast<bnb::Credential*>(ptr_platform->GetParent()))
            if (_delegate)
                _delegate->OnMovePair(ptr_credential->GetID(), ptr_platform->GetID(), m_Account.GetID(), id, offset);
}

void AccountView::OnSort(int cln, bool ascending)
{
    if (auto ptr_platform = dynamic_cast<bnb::platform_node*>(m_Account.GetParent()))
        if (auto ptr_credential = dynamic_cast<bnb::Credential*>(ptr_platform->GetParent()))
            if (_delegate)
                _delegate->OnSortPair(ptr_credential->GetID(), ptr_platform->GetID(), m_Account.GetID(), cln, ascending);
}

void AccountView::OnDoubleClicked(unsigned int id)
{
    if (auto ptr_platform = dynamic_cast<bnb::platform_node*>(m_Account.GetParent()))
    {
        if (auto ptr_credential = dynamic_cast<bnb::Credential*>(ptr_platform->GetParent()))
        {
            if (_delegate)
            {
                _delegate->SwitchToNode(ptr_credential->GetID(), ptr_platform->GetID(), m_Account.GetID(), id);
                _delegate->OnUpdatePair(ptr_credential->GetID(), ptr_platform->GetID(), m_Account.GetID(), id);
            }
        }
    }
}

void AccountView::OnClickedBack()
{
    if (auto ptr_platform = dynamic_cast<bnb::platform_node*>(m_Account.GetParent()))
        if (auto ptr_credential = dynamic_cast<bnb::Credential*>(ptr_platform->GetParent()))
            if (_delegate)
                _delegate->SwitchToNode(ptr_credential->GetID(), ptr_platform->GetID());
}

void AccountView::OnClickedEdit()
{
    if (auto ptr_platform = dynamic_cast<bnb::platform_node*>(m_Account.GetParent()))
        if (auto ptr_credential = dynamic_cast<bnb::Credential*>(ptr_platform->GetParent()))
            if (_delegate)
                _delegate->OnUpdateAccount(ptr_credential->GetID(), ptr_platform->GetID(), m_Account.GetID());
}

void AccountView::OnClickedRemove()
{
    if (auto ptr_platform = dynamic_cast<bnb::platform_node*>(m_Account.GetParent()))
        if (auto ptr_credential = dynamic_cast<bnb::Credential*>(ptr_platform->GetParent()))
            if (_delegate)
                _delegate->OnRemoveAccount(ptr_credential->GetID(), ptr_platform->GetID(), m_Account.GetID());
}

//------------------------------------------------------------------------------

template<>
void AccountView::base_type::ui_type::RetranslateUI(AccountView::base_type* pView)
{
    m_tabView->setColumnCount(2);
    m_tabView->setHorizontalHeaderLabels({ "        Key        ", "        Value        " });

    _labText[0]->setText("Account: ");
    _labText[1]->setText("Description: ");
}

template<>
void AccountView::base_type::ui_type::CreateLabel(AccountView::base_type* pView)
{
    for (unsigned int i = 0; i < 2; ++i)
    {
        _labText[i] = ui_utils::MakeStaticLabel(pView);
        m_editText[i] = ui_utils::MakeShowLine(pView, ui_utils::g_clrAccount);
    }
}

QT_END_NAMESPACE
