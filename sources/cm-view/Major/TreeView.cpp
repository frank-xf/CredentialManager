#include <QtGui/QMouseEvent>
#include <QtWidgets/QMenu>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QStyleFactory>

#include "Credential/Credential.h"

#include "credential_qt_string.h"
#include "credential_qt_utils.h"
#include "credential_qt_delegate.h"

#include "Major/TreeView.h"

QT_BEGIN_NAMESPACE

//------------------------------------------------------------------------------

static inline bnb::credential_enum GetItemType(const QTreeWidgetItem* item)
{
    return static_cast<bnb::credential_enum>((item->data(0, Qt::UserRole).toULongLong() >> 32) & 0xffffffff);
}

static inline bnb::Credential::id_type GetItemID(const QTreeWidgetItem* item)
{
    return static_cast<bnb::Credential::id_type>(item->data(0, Qt::UserRole).toULongLong() & 0xffffffff);
}

static inline unsigned long long MakeItemData(bnb::credential_enum t, bnb::Credential::id_type id)
{
    return (static_cast<unsigned long long>(t) << 32) | (id & 0xffffffff);
}

static inline QTreeWidgetItem* MakeTreeItem(QTreeWidgetItem* p, const QString& strText, bnb::credential_enum t, bnb::Credential::id_type id, const QColor& c)
{
    QTreeWidgetItem* pItem = new QTreeWidgetItem(p, { strText });
    pItem->setTextColor(0, c);
    pItem->setSizeHint(0, { ui_utils::tree_item_w, ui_utils::def_widget_h });
    pItem->setData(0, Qt::UserRole, MakeItemData(t, id));
    pItem->setFont(0, ui_utils::MakeFont());

    return pItem;
}

static inline QTreeWidgetItem* MoveItem(QTreeWidgetItem* parent, int i, int offset)
{
    auto child = parent->takeChild(i);

    int nIndex = i + offset;
    if (nIndex < 0) nIndex = 0;

    int nCount = parent->childCount();
    if (nCount < nIndex) nIndex = nCount;

    parent->insertChild(nIndex, child);
    return child;
}

static inline bool TransferItem(QTreeWidgetItem* parent, QList<QTreeWidgetItem*>& listItem, bnb::Credential::id_type id)
{
    for (int i = 0; i < listItem.size(); ++i)
    {
        auto pItem = listItem.at(i);
        if (GetItemID(pItem) == id)
        {
            parent->addChild(pItem);
            listItem.removeAt(i);
            return true;
        }
    }

    return false;
}

//==============================================================================
// Implementation of TreeView
//==============================================================================
TreeView::TreeView(DelegateType* pDelegate, QWidget * parent)
    : QTreeWidget(parent)
    , _delegate(pDelegate)
{
    _ui.SetupUI(this);

    setFocusPolicy(Qt::NoFocus);

    setMinimumWidth(ui_utils::tree_view_min_w);
    setMaximumWidth(ui_utils::tree_view_max_w);

    setStyle(QStyleFactory::create("Windows"));

    setContextMenuPolicy(Qt::CustomContextMenu);

    setHeaderHidden(true);
    setRootIsDecorated(false);

    setStyleSheet("QToolTip{ background:white; border:1px solid #C0C0C0; opacity:192; color:black; }\n"
        "QTreeView::item{ border: none; }\n"
        "QTreeView::item:hover{ background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #B0FFB0, stop:1 #F0FFF0); }\n"
        "QTreeView::item:selected:!active{ background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #20B020, stop:1 #F0FFF0); color:white; }");

    QObject::connect(this, &QTreeWidget::customContextMenuRequested, this, &TreeView::OnTreeContextMenu);
    QObject::connect(this, &QTreeWidget::currentItemChanged, this, &TreeView::OnItemChanged);

    QObject::connect(_ui.m_actAddAccount, &QAction::triggered, this, &TreeView::OnAddAccount);
    QObject::connect(_ui.m_actAddPlatform, &QAction::triggered, this, &TreeView::OnAddPlatform);
    QObject::connect(_ui.m_actAddPair, &QAction::triggered, this, &TreeView::OnAddPair);
    QObject::connect(_ui.m_actDelCredential, &QAction::triggered, this, &TreeView::OnRemoveCredential);
    QObject::connect(_ui.m_actDelAccount, &QAction::triggered, this, &TreeView::OnRemoveAccount);
    QObject::connect(_ui.m_actDelPlatform, &QAction::triggered, this, &TreeView::OnRemovePlatform);
    QObject::connect(_ui.m_actDelPair, &QAction::triggered, this, &TreeView::OnRemovePair);
    QObject::connect(_ui.m_actEditAccount, &QAction::triggered, this, &TreeView::OnEditAccount);
    QObject::connect(_ui.m_actEditPlatform, &QAction::triggered, this, &TreeView::OnEditPlatform);
    QObject::connect(_ui.m_actEditPair, &QAction::triggered, this, &TreeView::OnEditPair);
    QObject::connect(_ui.m_actModifyPassword, &QAction::triggered, this, &TreeView::OnMotifyPassword);
    QObject::connect(_ui.m_actEditCredential, &QAction::triggered, this, &TreeView::OnEditCredential);
}

void TreeView::ClearCredential()
{
    clear();
}

QTreeWidgetItem* TreeView::AddCredential(const bnb::Credential & credential)
{
    return (_AddCredential(credential));
}

QTreeWidgetItem* TreeView::AddPlatform(const bnb::platform_node& platform)
{
    if (auto ptr_credential = dynamic_cast<bnb::Credential*>(platform.GetParent()))
    {
        if (auto item_credential = FindItem(ptr_credential->GetID()))
        {
            item_credential->setExpanded(true);
            return (_AddPlatform(item_credential, platform));
        }
    }

    return nullptr;
}

QTreeWidgetItem* TreeView::AddAccount(const bnb::account_node& account)
{
    if (auto ptr_platform = dynamic_cast<bnb::platform_node*>(account.GetParent()))
    {
        if (auto ptr_credential = dynamic_cast<bnb::Credential*>(ptr_platform->GetParent()))
        {
            if (auto item_platform = FindItem(ptr_credential->GetID(), ptr_platform->GetID()))
            {
                item_platform->setExpanded(true);
                return (_AddAccount(item_platform, account));
            }
        }
    }

    return nullptr;
}

QTreeWidgetItem* TreeView::AddPair(const bnb::pair_node& pair)
{
    if (auto ptr_account = dynamic_cast<bnb::account_node*>(pair.GetParent()))
    {
        if (auto ptr_platform = dynamic_cast<bnb::platform_node*>(ptr_account->GetParent()))
        {
            if (auto ptr_credential = dynamic_cast<bnb::Credential*>(ptr_platform->GetParent()))
            {
                if (auto item_account = FindItem(ptr_credential->GetID(), ptr_platform->GetID(), ptr_account->GetID()))
                {
                    item_account->setExpanded(true);
                    return (_AddPair(item_account, pair));
                }
            }
        }
    }

    return nullptr;
}

QTreeWidgetItem* TreeView::UpdateCredential(const bnb::Credential& credential)
{
    if (auto item_credential = FindItem(credential.GetID()))
    {
        item_credential->setText(0, '[' + To_QString(credential.GetData().GetUser()) + ']');
        return item_credential;
    }

    return nullptr;
}

QTreeWidgetItem* TreeView::UpdatePlatform(const bnb::platform_node & platform)
{
    if (auto ptr_credential = dynamic_cast<bnb::Credential*>(platform.GetParent()))
    {
        if (auto item_platform = FindItem(ptr_credential->GetID(), platform.GetID()))
        {
            item_platform->setText(0, To_QString(platform.GetData().GetName()));
            return item_platform;
        }
    }

    return nullptr;
}

QTreeWidgetItem* TreeView::UpdateAccount(const bnb::account_node & account)
{
    if (auto ptr_platform = dynamic_cast<bnb::platform_node*>(account.GetParent()))
    {
        if (auto ptr_credential = dynamic_cast<bnb::Credential*>(ptr_platform->GetParent()))
        {
            if (auto item_account = FindItem(ptr_credential->GetID(), ptr_platform->GetID(), account.GetID()))
            {
                item_account->setText(0, To_QString(account.GetData().GetName()));
                return item_account;
            }
        }
    }

    return nullptr;
}

QTreeWidgetItem* TreeView::UpdatePair(const bnb::pair_node & pair)
{
    if (auto ptr_account = dynamic_cast<bnb::account_node*>(pair.GetParent()))
    {
        if (auto ptr_platform = dynamic_cast<bnb::platform_node*>(ptr_account->GetParent()))
        {
            if (auto ptr_credential = dynamic_cast<bnb::Credential*>(ptr_platform->GetParent()))
            {
                if (auto item_pair = FindItem(ptr_credential->GetID(), ptr_platform->GetID(), ptr_account->GetID(), pair.GetID()))
                {
                    item_pair->setText(0, To_QString(pair.GetData().GetKey()));
                    return item_pair;
                }
            }
        }
    }

    return nullptr;
}

QTreeWidgetItem* TreeView::MovePlatform(id_type credentialId, id_type platformId, int offset)
{
    if (auto item_credential = FindItem(credentialId))
    {
        for (int i = 0; i < item_credential->childCount(); ++i)
        {
            auto item_platform = item_credential->child(i);
            if (platformId == GetItemID(item_platform))
                return MoveItem(item_credential, i, offset);
        }
    }

    return nullptr;
}

QTreeWidgetItem* TreeView::MoveAccount(id_type credentialId, id_type platformId, id_type accountId, int offset)
{
    if (auto item_platform = FindItem(credentialId, platformId))
    {
        for (int i = 0; i < item_platform->childCount(); ++i)
        {
            auto item_account = item_platform->child(i);
            if (accountId == GetItemID(item_account))
                return MoveItem(item_platform, i, offset);
        }
    }

    return nullptr;
}

QTreeWidgetItem* TreeView::MovePair(id_type credentialId, id_type platformId, id_type accountId, id_type pairId, int offset)
{
    if (auto item_account = FindItem(credentialId, platformId, accountId))
    {
        for (int i = 0; i < item_account->childCount(); ++i)
        {
            auto item_pair = item_account->child(i);
            if (pairId == GetItemID(item_pair))
                return MoveItem(item_account, i, offset);
        }
    }

    return nullptr;
}

bool TreeView::RemoveCredential(id_type credentialId)
{
    for (int i = 0; i < topLevelItemCount(); ++i)
    {
        if (credentialId == GetItemID(topLevelItem(i)))
        {
            auto item_credential = takeTopLevelItem(i);
            delete item_credential;

            return true;
        }
    }

    return false;
}

bool TreeView::RemovePlatform(id_type credentialId, id_type platformId)
{
    if (auto item_credential = FindItem(credentialId))
    {
        if (auto item_platform = _FindItem(item_credential, platformId))
        {
            item_credential->removeChild(item_platform);
            delete item_platform;

            return true;
        }
    }

    return false;
}

bool TreeView::RemoveAccount(id_type credentialId, id_type platformId, id_type accountId)
{
    if (auto item_platform = FindItem(credentialId, platformId))
    {
        if (auto item_account = _FindItem(item_platform, accountId))
        {
            item_platform->removeChild(item_account);
            delete item_account;

            return true;
        }
    }

    return false;
}

bool TreeView::RemovePair(id_type credentialId, id_type platformId, id_type accountId, id_type pairId)
{
    if (auto item_account = FindItem(credentialId, platformId, accountId))
    {
        if (auto item_pair = _FindItem(item_account, pairId))
        {
            item_account->removeChild(item_pair);
            delete item_pair;

            return true;
        }
    }

    return false;
}

bool TreeView::Reschedule(const bnb::Credential & credential)
{
    if (auto item_credential = FindItem(credential.GetID()))
    {
        auto listItem = item_credential->takeChildren();
        credential.PreorderTraversal([&listItem, &item_credential](const bnb::platform_node& item) mutable {
            TransferItem(item_credential, listItem, item.GetID());
        });

        return true;
    }

    return false;
}

bool TreeView::Reschedule(const bnb::platform_node & platform)
{
    if (auto ptr_credential = dynamic_cast<bnb::Credential*>(platform.GetParent()))
    {
        if (auto item_platform = FindItem(ptr_credential->GetID(), platform.GetID()))
        {
            auto listItem = item_platform->takeChildren();
            platform.PreorderTraversal([&listItem, &item_platform](const bnb::account_node& item) mutable {
                TransferItem(item_platform, listItem, item.GetID());
            });

            return true;
        }
    }

    return false;
}

bool TreeView::Reschedule(const bnb::account_node & account)
{
    if (auto ptr_platform = dynamic_cast<bnb::platform_node*>(account.GetParent()))
    {
        if (auto ptr_credential = dynamic_cast<bnb::Credential*>(ptr_platform->GetParent()))
        {
            if (auto item_account = FindItem(ptr_credential->GetID(), ptr_platform->GetID(), account.GetID()))
            {
                auto listItem = item_account->takeChildren();
                account.PreorderTraversal([&listItem, &item_account](const bnb::pair_node& item) mutable {
                    TransferItem(item_account, listItem, item.GetID());
                });

                return true;
            }
        }
    }

    return false;
}

QTreeWidgetItem* TreeView::FindItem(id_type id)
{
    for (int i = 0; i < topLevelItemCount(); ++i)
    {
        auto child = topLevelItem(i);
        if (id == GetItemID(child))
            return child;
    }

    return nullptr;
}

void TreeView::mouseDoubleClickEvent(QMouseEvent * event)
{
    if (Qt::LeftButton == event->button())
    {
        if (auto pItem = itemAt(event->pos()))
        {
            switch (GetItemType(pItem))
            {
            case bnb::credential_enum::credential:
                _EditCredential(pItem);
                break;
            case bnb::credential_enum::platform:
                _EditPlatform(pItem);
                break;
            case bnb::credential_enum::account:
                _EditAccount(pItem);
                break;
            case bnb::credential_enum::pair:
                _EditPair(pItem);
                break;
            default:
                break;
            }

            event->accept();
            return;
        }
    }

    QTreeWidget::mouseDoubleClickEvent(event);
}

void TreeView::OnTreeContextMenu(const QPoint & pos)
{
    if (QTreeWidgetItem* pItem = itemAt(pos))
    {
        QMenu treeMenu(this);
        treeMenu.setStyleSheet("QMenu{ border: 1px solid gray; background-color: white; }"
            "QMenu::item{ padding: 4px 20px 4px 20px; }"
            "QMenu::item:selected{ background:#C0F0C0;}"
            "QMenu::separator{ height: 1px; background: gray; margin-left: 2px; margin-right: 2px;}");

        switch (GetItemType(pItem))
        {
        case bnb::credential_enum::credential:
            treeMenu.addAction(_ui.m_actEditCredential);
            treeMenu.addAction(_ui.m_actModifyPassword);
            treeMenu.addAction(_ui.m_actDelCredential);
            treeMenu.addSeparator();
            treeMenu.addAction(_ui.m_actAddPlatform);
            break;
        case bnb::credential_enum::pair:
            treeMenu.addAction(_ui.m_actEditPair);
            treeMenu.addAction(_ui.m_actDelPair);
            break;
        case bnb::credential_enum::account:
            treeMenu.addAction(_ui.m_actEditAccount);
            treeMenu.addAction(_ui.m_actDelAccount);
            treeMenu.addSeparator();
            treeMenu.addAction(_ui.m_actAddPair);
            break;
        case bnb::credential_enum::platform:
            treeMenu.addAction(_ui.m_actEditPlatform);
            treeMenu.addAction(_ui.m_actDelPlatform);
            treeMenu.addSeparator();
            treeMenu.addAction(_ui.m_actAddAccount);
            break;
        default:
            break;
        }

        treeMenu.exec(QCursor::pos());
    }
}

void TreeView::OnItemChanged(QTreeWidgetItem * cur, QTreeWidgetItem * pre)
{
    if (cur)
    {
        bnb::credential_enum eType = GetItemType(cur);
        id_type id = GetItemID(cur);

        switch (eType)
        {
        case bnb::credential_enum::pair:
        {
            if (auto item_account = cur->parent())
            {
                if (bnb::credential_enum::account == GetItemType(item_account))
                {
                    eType = bnb::credential_enum::account;
                    id = GetItemID(item_account);

                    break;
                }
            }

            return;
        }
        case bnb::credential_enum::account:
        case bnb::credential_enum::platform:
        case bnb::credential_enum::credential:
            break;
        default:
            return;
        }

        if (_delegate)
            _delegate->SwitchToView(static_cast<unsigned int>(eType), id);
    }
}

void TreeView::OnAddPlatform()
{
    if (QTreeWidgetItem* item_credential = currentItem())
        if (bnb::credential_enum::credential == GetItemType(item_credential))
            if (_delegate)
                _delegate->OnAddPlatform(GetItemID(item_credential));
}

void TreeView::OnAddAccount()
{
    if (QTreeWidgetItem* item_platform = currentItem())
        if (bnb::credential_enum::platform == GetItemType(item_platform))
            if (QTreeWidgetItem* item_credential = item_platform->parent())
                if (bnb::credential_enum::credential == GetItemType(item_credential))
                    if (_delegate)
                        _delegate->OnAddAccount(GetItemID(item_credential), GetItemID(item_platform));
}

void TreeView::OnAddPair()
{
    if (QTreeWidgetItem* item_account = currentItem())
        if (bnb::credential_enum::account == GetItemType(item_account))
            if (QTreeWidgetItem* item_platform = item_account->parent())
                if (bnb::credential_enum::platform == GetItemType(item_platform))
                    if (QTreeWidgetItem* item_credential = item_platform->parent())
                        if (bnb::credential_enum::credential == GetItemType(item_credential))
                            if (_delegate)
                                _delegate->OnAddPair(GetItemID(item_credential), GetItemID(item_platform), GetItemID(item_account));
}

void TreeView::OnMotifyPassword()
{
    if (QTreeWidgetItem* item_credential = currentItem())
        if (bnb::credential_enum::credential == GetItemType(item_credential))
            if (_delegate)
                _delegate->OnUpdatePassword(GetItemID(item_credential));
}

void TreeView::OnEditCredential()
{
    if (QTreeWidgetItem* item_credential = currentItem())
        if (bnb::credential_enum::credential == GetItemType(item_credential))
            _EditCredential(item_credential);
}

void TreeView::OnEditPlatform()
{
    if (QTreeWidgetItem* item_platform = currentItem())
        if (bnb::credential_enum::platform == GetItemType(item_platform))
            _EditPlatform(item_platform);
}

void TreeView::OnEditAccount()
{
    if (QTreeWidgetItem* item_account = currentItem())
        if (bnb::credential_enum::account == GetItemType(item_account))
            _EditAccount(item_account);
}

void TreeView::OnEditPair()
{
    if (QTreeWidgetItem* item_pair = currentItem())
        if (bnb::credential_enum::pair == GetItemType(item_pair))
            _EditPair(item_pair);
}

void TreeView::OnRemovePlatform()
{
    if (QTreeWidgetItem* item_platform = currentItem())
        if (bnb::credential_enum::platform == GetItemType(item_platform))
            if (QTreeWidgetItem* item_credential = item_platform->parent())
                if (bnb::credential_enum::credential == GetItemType(item_credential))
                    if (_delegate)
                        _delegate->OnRemovePlatform(GetItemID(item_credential), GetItemID(item_platform));
}

void TreeView::OnRemoveAccount()
{
    if (QTreeWidgetItem* item_account = currentItem())
        if (bnb::credential_enum::account == GetItemType(item_account))
            if (QTreeWidgetItem* item_platform = item_account->parent())
                if (bnb::credential_enum::platform == GetItemType(item_platform))
                    if (QTreeWidgetItem* item_credential = item_platform->parent())
                        if (bnb::credential_enum::credential == GetItemType(item_credential))
                            if (_delegate)
                                _delegate->OnRemoveAccount(GetItemID(item_credential), GetItemID(item_platform), GetItemID(item_account));
}

void TreeView::OnRemovePair()
{
    if (QTreeWidgetItem* item_pair = currentItem())
        if (bnb::credential_enum::pair == GetItemType(item_pair))
            if (QTreeWidgetItem* item_account = item_pair->parent())
                if (bnb::credential_enum::account == GetItemType(item_account))
                    if (QTreeWidgetItem* item_platform = item_account->parent())
                        if (bnb::credential_enum::platform == GetItemType(item_platform))
                            if (QTreeWidgetItem* item_credential = item_platform->parent())
                                if (bnb::credential_enum::credential == GetItemType(item_credential))
                                    if (_delegate)
                                        _delegate->OnRemovePair(GetItemID(item_credential), GetItemID(item_platform), GetItemID(item_account), GetItemID(item_pair));
}

void TreeView::OnRemoveCredential()
{
    if (QTreeWidgetItem* item_credential = currentItem())
        if (bnb::credential_enum::credential == GetItemType(item_credential))
            if (_delegate)
                _delegate->OnRemoveCredential(GetItemID(item_credential));
}

QTreeWidgetItem* TreeView::_AddCredential(const bnb::Credential & credential)
{
    QTreeWidgetItem* item_credential = new QTreeWidgetItem(this, { '[' + To_QString(credential.GetData().GetUser()) + ']' });
    item_credential->setTextColor(0, ui_utils::g_clrCredential);
    item_credential->setSizeHint(0, { ui_utils::tree_item_w, ui_utils::def_widget_h });
    item_credential->setData(0, Qt::UserRole, MakeItemData(credential.GetData().GetType(), credential.GetID()));
    item_credential->setFont(0, ui_utils::MakeFont());
    addTopLevelItem(item_credential);

    credential.PreorderTraversal([this, item_credential](const bnb::platform_node& platform) {
        auto item_platform = _AddPlatform(item_credential, platform);

        platform.PreorderTraversal([this, item_platform](const bnb::account_node& account) {
            auto item_account = _AddAccount(item_platform, account);

            account.PreorderTraversal([this, item_account](const bnb::pair_node& pair) {
                _AddPair(item_account, pair);
            });
        });
    });

    item_credential->setExpanded(true);
    return item_credential;
}

QTreeWidgetItem* TreeView::_AddPlatform(QTreeWidgetItem * parent, const bnb::platform_node& platform)
{
    auto item_platform = MakeTreeItem(parent, To_QString(platform.GetData().GetName()), platform.GetData().GetType(), platform.GetID(), ui_utils::g_clrPlatform);
    parent->addChild(item_platform);

    return item_platform;
}

QTreeWidgetItem* TreeView::_AddAccount(QTreeWidgetItem * parent, const bnb::account_node& account)
{
    auto item_account = MakeTreeItem(parent, To_QString(account.GetData().GetName()), account.GetData().GetType(), account.GetID(), ui_utils::g_clrAccount);
    parent->addChild(item_account);

    return item_account;
}

QTreeWidgetItem* TreeView::_AddPair(QTreeWidgetItem * parent, const bnb::pair_node& pair)
{
    auto item_pair = MakeTreeItem(parent, To_QString(pair.GetData().GetKey()), pair.GetData().GetType(), pair.GetID(), ui_utils::g_clrPair);
    parent->addChild(item_pair);

    return item_pair;
}

void TreeView::_EditCredential(QTreeWidgetItem * item_credential)
{
    if (_delegate)
        _delegate->OnUpdateCredential(GetItemID(item_credential));
}

void TreeView::_EditPlatform(QTreeWidgetItem * item_platform)
{
    if (QTreeWidgetItem* item_credential = item_platform->parent())
        if (bnb::credential_enum::credential == GetItemType(item_credential))
            if (_delegate)
                _delegate->OnUpdatePlatform(GetItemID(item_credential), GetItemID(item_platform));
}

void TreeView::_EditAccount(QTreeWidgetItem * item_account)
{
    if (QTreeWidgetItem* item_platform = item_account->parent())
        if (bnb::credential_enum::platform == GetItemType(item_platform))
            if (QTreeWidgetItem* item_credential = item_platform->parent())
                if (bnb::credential_enum::credential == GetItemType(item_credential))
                    if (_delegate)
                        _delegate->OnUpdateAccount(GetItemID(item_credential), GetItemID(item_platform), GetItemID(item_account));
}

void TreeView::_EditPair(QTreeWidgetItem * item_pair)
{
    if (QTreeWidgetItem* item_account = item_pair->parent())
        if (bnb::credential_enum::account == GetItemType(item_account))
            if (QTreeWidgetItem* item_platform = item_account->parent())
                if (bnb::credential_enum::platform == GetItemType(item_platform))
                    if (QTreeWidgetItem* item_credential = item_platform->parent())
                        if (bnb::credential_enum::credential == GetItemType(item_credential))
                            if (_delegate)
                                _delegate->OnUpdatePair(GetItemID(item_credential), GetItemID(item_platform), GetItemID(item_account), GetItemID(item_pair));
}

QTreeWidgetItem* TreeView::_FindItem(QTreeWidgetItem * parent, bnb::Credential::id_type id)
{
    if (parent)
    {
        for (int i = 0; i < parent->childCount(); ++i)
        {
            auto child = parent->child(i);
            if (id == GetItemID(child))
                return child;
        }
    }

    return nullptr;
}

//------------------------------------------------------------------------------

void TreeView::ui_type::SetupUI(TreeView* pView)
{
    m_actAddAccount = new QAction(pView);
    m_actAddPlatform = new QAction(pView);
    m_actAddPair = new QAction(pView);
    m_actDelAccount = new QAction(pView);
    m_actDelPlatform = new QAction(pView);
    m_actDelPair = new QAction(pView);
    m_actEditAccount = new QAction(pView);
    m_actEditPlatform = new QAction(pView);
    m_actEditPair = new QAction(pView);
    m_actModifyPassword = new QAction(pView);
    m_actEditCredential = new QAction(pView);
    m_actDelCredential = new QAction(pView);

    RetranslateUI(pView);
}

void TreeView::ui_type::RetranslateUI(TreeView * pView)
{
    m_actAddAccount->setText("Add Account");
    m_actAddPlatform->setText("Add Platform");
    m_actAddPair->setText("Add Pair");
    m_actDelAccount->setText("Remove Account");
    m_actDelPlatform->setText("Remove Platform");
    m_actDelPair->setText("Remove Pair");
    m_actEditAccount->setText("Edit Account");
    m_actEditPlatform->setText("Edit Platform");
    m_actEditPair->setText("Edit Pair");
    m_actModifyPassword->setText("Modify Password");
    m_actEditCredential->setText("Edit Credential");
    m_actDelCredential->setText("Remove Credential");
}

QT_END_NAMESPACE
