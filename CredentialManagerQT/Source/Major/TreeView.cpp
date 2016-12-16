#include <QtGui/QMouseEvent>
#include <QtWidgets/QMenu>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QStyleFactory>
#include <QtWidgets/QStyledItemDelegate>

#include <iostream>

#include "Credential/Credential.h"

#include "credential_qt_string.h"
#include "credential_qt_utils.h"
#include "credential_qt_delegate.h"

#include "Widget/NoFocusDelegate.h"
#include "Major/TreeView.h"

QT_BEGIN_NAMESPACE

//------------------------------------------------------------------------------

static inline unsigned long long MakeItemData(bnb::credential_enum t, unsigned int id)
{
    return (static_cast<unsigned int>(t) << 32) | (id & 0xffffffff);
}

static inline bnb::credential_enum GetItemType(const QTreeWidgetItem* item)
{
    return static_cast<bnb::credential_enum>((item->data(0, Qt::UserRole).toULongLong() >> 32) & 0xffffffff);
}

static inline unsigned int GetItemID(const QTreeWidgetItem* item)
{
    return static_cast<unsigned int>(item->data(0, Qt::UserRole).toULongLong() & 0xffffffff);
}

static inline QTreeWidgetItem* MakeTreeItem(QTreeWidgetItem* p, const QString& strText, bnb::credential_enum t, unsigned int id, const QColor& c)
{
    QTreeWidgetItem* pItem = new QTreeWidgetItem(p, { strText });
    pItem->setTextColor(0, c);
    pItem->setSizeHint(0, { ui_utils::tree_item_w, ui_utils::def_widget_h });
    pItem->setData(0, Qt::UserRole, MakeItemData(t, id));
    pItem->setFont(0, ui_utils::MakeFont());

    return pItem;
}

//==============================================================================
// Implementation of TreeView
//==============================================================================
QTreeWidgetItem * TreeView::AddPlatform(QTreeWidgetItem * parent, const bnb::platform_node& pp)
{
    auto item_platform = MakeTreeItem(parent, To_QString(pp.GetData().GetName()), pp.GetData().GetType(), pp.GetID(), ui_utils::g_clrPlatform);
    parent->addChild(item_platform);

    return item_platform;
}

QTreeWidgetItem * TreeView::AddAccount(QTreeWidgetItem * parent, const bnb::account_node& pa)
{
    auto item_account = MakeTreeItem(parent, To_QString(pa.GetData().GetName()), pa.GetData().GetType(), pa.GetID(), ui_utils::g_clrAccount);
    parent->addChild(item_account);

    return item_account;
}

QTreeWidgetItem * TreeView::AddProperty(QTreeWidgetItem * parent, const bnb::property_node& pp)
{
    auto item_property = MakeTreeItem(parent, To_QString(pp.GetData().GetKey()), pp.GetData().GetType(), pp.GetID(), ui_utils::g_clrProperty);
    parent->addChild(item_property);

    return item_property;
}

void TreeView::_EditCredential(QTreeWidgetItem * item_credential)
{
    if (_delegate)
        _delegate->OnUpdateCredential(GetItemID(item_credential));
}

void TreeView::_EditPlatform(QTreeWidgetItem * item_platform)
{
    if (QTreeWidgetItem* item_credential = item_platform->parent())
        if (bnb::credential_enum::ct_credential == GetItemType(item_credential))
            if (_delegate)
                _delegate->OnUpdatePlatform(GetItemID(item_credential), GetItemID(item_platform));
}

void TreeView::_EditAccount(QTreeWidgetItem * item_account)
{
    if (QTreeWidgetItem* item_platform = item_account->parent())
        if (bnb::credential_enum::ct_platform == GetItemType(item_platform))
            if (QTreeWidgetItem* item_credential = item_platform->parent())
                if (bnb::credential_enum::ct_credential == GetItemType(item_credential))
                    if (_delegate)
                        _delegate->OnUpdateAccount(GetItemID(item_credential), GetItemID(item_platform), GetItemID(item_account));
}

void TreeView::_EditProperty(QTreeWidgetItem * item_property)
{
    if (QTreeWidgetItem* item_account = item_property->parent())
        if (bnb::credential_enum::ct_account == GetItemType(item_account))
            if (QTreeWidgetItem* item_platform = item_account->parent())
                if (bnb::credential_enum::ct_platform == GetItemType(item_platform))
                    if (QTreeWidgetItem* item_credential = item_platform->parent())
                        if (bnb::credential_enum::ct_credential == GetItemType(item_credential))
                            if (_delegate)
                                _delegate->OnUpdateProperty(GetItemID(item_credential), GetItemID(item_platform), GetItemID(item_account), GetItemID(item_property));
}

QTreeWidgetItem * TreeView::FindItem(unsigned int id)
{
    auto item_root = topLevelItem(0);
    if (id == GetItemID(item_root))
        return item_root;

    return nullptr;
}

QTreeWidgetItem * TreeView::FindItem(QTreeWidgetItem * parent, unsigned int id)
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

TreeView::TreeView(QWidget * parent) : QTreeWidget(parent)
{
    setItemDelegate(new NoFocusDelegate);

    setMinimumWidth(ui_utils::tree_view_min_w);
    setMaximumWidth(ui_utils::tree_view_max_w);

	setStyle(QStyleFactory::create("Windows"));

	setContextMenuPolicy(Qt::CustomContextMenu);

    setHeaderHidden(true);
    setRootIsDecorated(false);

    setStyleSheet(
        "QToolTip{ background:white; border:1px solid #C0C0C0; opacity:192; color:black; }\n"
        "QTreeView::item{ border: none; }\n"
        "QTreeView::item:hover{ background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #B0FFB0, stop:1 #F0FFF0); }\n"
        "QTreeView::item:selected{ background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #F04020, stop:1 #F0FFF0); }\n"
        "QTreeView::item:selected:active{ background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #FF2040, stop:1 #F0FFF0); color:white; }\n"
        "QTreeView::item:selected:!active{ background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #20B020, stop:1 #F0FFF0); color:white; }");

    QObject::connect(this, &QTreeWidget::customContextMenuRequested, this, &TreeView::OnTreeContextMenu);
    QObject::connect(this, &QTreeWidget::currentItemChanged, this, &TreeView::OnItemChanged);

    QObject::connect(_ui.m_actAddAccount, &QAction::triggered, this, &TreeView::OnAddAccount);
    QObject::connect(_ui.m_actAddPlatform, &QAction::triggered, this, &TreeView::OnAddPlatform);
    QObject::connect(_ui.m_actAddProperty, &QAction::triggered, this, &TreeView::OnAddProperty);
    QObject::connect(_ui.m_actDelAccount, &QAction::triggered, this, &TreeView::OnRemoveAccount);
    QObject::connect(_ui.m_actDelPlatform, &QAction::triggered, this, &TreeView::OnRemovePlatform);
    QObject::connect(_ui.m_actDelProperty, &QAction::triggered, this, &TreeView::OnRemoveProperty);
    QObject::connect(_ui.m_actEditAccount, &QAction::triggered, this, &TreeView::OnEditAccount);
    QObject::connect(_ui.m_actEditPlatform, &QAction::triggered, this, &TreeView::OnEditPlatform);
    QObject::connect(_ui.m_actEditProperty, &QAction::triggered, this, &TreeView::OnEditProperty);
    QObject::connect(_ui.m_actModifyPassword, &QAction::triggered, this, &TreeView::OnMotifyPassword);
    QObject::connect(_ui.m_actEditCredential, &QAction::triggered, this, &TreeView::OnEditCredential);
}

void TreeView::InitCredential(const bnb::Credential & credential)
{
    QTreeWidgetItem* item_root = new QTreeWidgetItem(this, { '[' + To_QString(credential.GetData().GetUser()) + ']' });
    item_root->setTextColor(0, ui_utils::g_clrCredential);
    item_root->setSizeHint(0, { ui_utils::tree_item_w, ui_utils::def_widget_h });
    item_root->setData(0, Qt::UserRole, MakeItemData(credential.GetData().GetType(), credential.GetID()));
    item_root->setFont(0, ui_utils::MakeFont());
    addTopLevelItem(item_root);

    credential.PreorderTraversal([this, item_root](const bnb::platform_node& platform) {
        auto item_platform = AddPlatform(item_root, platform);

        platform.PreorderTraversal([this, item_platform](const bnb::account_node& account) {
            auto item_account = AddAccount(item_platform, account);

            account.PreorderTraversal([this, item_account](const bnb::property_node& property) {
                AddProperty(item_account, property);
            });
        });

        item_platform->setExpanded(true);
    });

    item_root->setExpanded(true);
}

void TreeView::ClearCredential()
{
	clear();
}

void TreeView::UpdateHeader(const QString& strText)
{
    QTreeWidgetItem* pItem = topLevelItem(0);

    if (pItem)
        pItem->setText(0, '[' + strText + ']');
}

QTreeWidgetItem * TreeView::AddPlatform(const bnb::platform_node& pp)
{
    if (auto ptr_credential = dynamic_cast<bnb::Credential*>(pp.GetParent()))
    {
        if (auto item_credential = FindItem(ptr_credential->GetID()))
        {
            item_credential->setExpanded(true);
            return AddPlatform(item_credential, pp);
        }
    }

    return nullptr;
}

QTreeWidgetItem* TreeView::AddAccount(const bnb::account_node& pa)
{
    if (auto ptr_platform = dynamic_cast<bnb::platform_node*>(pa.GetParent()))
    {
        if (auto ptr_credential = dynamic_cast<bnb::Credential*>(ptr_platform->GetParent()))
        {
            if (auto item_platform = FindItem(FindItem(ptr_credential->GetID()), ptr_platform->GetID()))
            {
                item_platform->setExpanded(true);                
                return AddAccount(item_platform, pa);
            }
        }
    }

    return nullptr;
}

QTreeWidgetItem* TreeView::AddProperty(const bnb::property_node& pp)
{
    if (auto ptr_account = dynamic_cast<bnb::account_node*>(pp.GetParent()))
    {
        if (auto ptr_platform = dynamic_cast<bnb::platform_node*>(ptr_account->GetParent()))
        {
            if (auto ptr_credential = dynamic_cast<bnb::Credential*>(ptr_platform->GetParent()))
            {
                if (auto item_account = FindItem(FindItem(FindItem(ptr_credential->GetID()), ptr_platform->GetID()), ptr_account->GetID()))
                {
                    item_account->setExpanded(true);
                    return AddProperty(item_account, pp);
                }
            }
        }
    }

    return nullptr;
}

QTreeWidgetItem * TreeView::UpdateProperty(const bnb::property_node & pp)
{
    if (auto ptr_account = dynamic_cast<bnb::account_node*>(pp.GetParent()))
    {
        if (auto ptr_platform = dynamic_cast<bnb::platform_node*>(ptr_account->GetParent()))
        {
            if (auto ptr_credential = dynamic_cast<bnb::Credential*>(ptr_platform->GetParent()))
            {
                if (auto item_property = FindItem(FindItem(FindItem(FindItem(ptr_credential->GetID()), ptr_platform->GetID()), ptr_account->GetID()), pp.GetID()))
                {
                    item_property->setText(0, To_QString(pp.GetData().GetKey()));
                    return item_property;
                }
            }
        }
    }

    return nullptr;
}

bool TreeView::RemovePlatform(unsigned int id1, unsigned int id2)
{
    if (auto item_credential = FindItem(id1))
    {
        if (auto item_platform = FindItem(item_credential, id2))
        {
            item_credential->removeChild(item_platform);
            delete item_platform;

            return true;
        }
    }

    return false;
}

bool TreeView::RemoveAccount(unsigned int id1, unsigned int id2, unsigned int id3)
{
    if (auto item_platform = FindItem(FindItem(id1), id2))
    {
        if (auto item_account = FindItem(item_platform, id3))
        {
            item_platform->removeChild(item_account);
            delete item_account;

            return true;
        }
    }

    return false;
}

bool TreeView::RemoveProperty(unsigned int id1, unsigned int id2, unsigned int id3, unsigned int id4)
{
    if (auto item_account = FindItem(FindItem(FindItem(id1), id2), id3))
    {
        if (auto item_property = FindItem(item_account, id4))
        {
            item_account->removeChild(item_property);
            delete item_property;

            return true;
        }
    }

    return false;
}

void TreeView::OnAddPlatform()
{
    if (QTreeWidgetItem* item_credential = currentItem())
        if (bnb::credential_enum::ct_credential == GetItemType(item_credential))
            if (_delegate)
                _delegate->OnAddPlatform(GetItemID(item_credential));
}

void TreeView::OnAddAccount()
{
    if (QTreeWidgetItem* item_platform = currentItem())
        if (bnb::credential_enum::ct_platform == GetItemType(item_platform))
            if (QTreeWidgetItem* item_credential = item_platform->parent())
                if (bnb::credential_enum::ct_credential == GetItemType(item_credential))
                    if (_delegate)
                        _delegate->OnAddAccount(GetItemID(item_credential), GetItemID(item_platform));
}

void TreeView::OnAddProperty()
{
    if (QTreeWidgetItem* item_account = currentItem())
        if (bnb::credential_enum::ct_account == GetItemType(item_account))
            if (QTreeWidgetItem* item_platform = item_account->parent())
                if (bnb::credential_enum::ct_platform == GetItemType(item_platform))
                    if (QTreeWidgetItem* item_credential = item_platform->parent())
                        if (bnb::credential_enum::ct_credential == GetItemType(item_credential))
                            if (_delegate)
                                _delegate->OnAddProperty(GetItemID(item_credential), GetItemID(item_platform), GetItemID(item_account));
}

void TreeView::OnMotifyPassword()
{
    if (QTreeWidgetItem* item_credential = currentItem())
        if (bnb::credential_enum::ct_credential == GetItemType(item_credential))
            if (_delegate)
                _delegate->OnUpdatePassword(GetItemID(item_credential));
}

void TreeView::OnEditCredential()
{
    if (QTreeWidgetItem* item_credential = currentItem())
        if (bnb::credential_enum::ct_credential == GetItemType(item_credential))
            _EditCredential(item_credential);
}

void TreeView::OnEditPlatform()
{
    if (QTreeWidgetItem* item_platform = currentItem())
        if (bnb::credential_enum::ct_platform == GetItemType(item_platform))
            _EditPlatform(item_platform);
}

void TreeView::OnEditAccount()
{
    if (QTreeWidgetItem* item_account = currentItem())
        if (bnb::credential_enum::ct_account == GetItemType(item_account))
            _EditAccount(item_account);
}

void TreeView::OnEditProperty()
{
    if (QTreeWidgetItem* item_property = currentItem())
        if (bnb::credential_enum::ct_property == GetItemType(item_property))
            _EditProperty(item_property);
}

void TreeView::OnRemovePlatform()
{
    if (QTreeWidgetItem* item_platform = currentItem())
        if (bnb::credential_enum::ct_platform == GetItemType(item_platform))
            if (QTreeWidgetItem* item_credential = item_platform->parent())
                if (bnb::credential_enum::ct_credential == GetItemType(item_credential))
                    if (_delegate)
                        _delegate->OnRemovePlatform(GetItemID(item_credential), GetItemID(item_platform));
}

void TreeView::OnRemoveAccount()
{
    if (QTreeWidgetItem* item_account = currentItem())
        if (bnb::credential_enum::ct_account == GetItemType(item_account))
            if (QTreeWidgetItem* item_platform = item_account->parent())
                if (bnb::credential_enum::ct_platform == GetItemType(item_platform))
                    if (QTreeWidgetItem* item_credential = item_platform->parent())
                        if (bnb::credential_enum::ct_credential == GetItemType(item_credential))
                            if (_delegate)
                                _delegate->OnRemoveAccount(GetItemID(item_credential), GetItemID(item_platform), GetItemID(item_account));
}

void TreeView::OnRemoveProperty()
{
    if (QTreeWidgetItem* item_property = currentItem())
        if (bnb::credential_enum::ct_property == GetItemType(item_property))
            if (QTreeWidgetItem* item_account = item_property->parent())
                if (bnb::credential_enum::ct_account == GetItemType(item_account))
                    if (QTreeWidgetItem* item_platform = item_account->parent())
                        if (bnb::credential_enum::ct_platform == GetItemType(item_platform))
                            if (QTreeWidgetItem* item_credential = item_platform->parent())
                                if (bnb::credential_enum::ct_credential == GetItemType(item_credential))
                                    if (_delegate)
                                        _delegate->OnRemoveProperty(GetItemID(item_credential), GetItemID(item_platform), GetItemID(item_account), GetItemID(item_property));
}

void TreeView::OnTreeContextMenu(const QPoint & pos)
{
    QTreeWidgetItem* pItem = itemAt(pos);

    QMenu treeMenu(this);
    treeMenu.setStyleSheet("QMenu{ border: 1px solid gray; background-color: white; }"
        "QMenu::item{ padding: 8px 20px 8px 20px; }"
        "QMenu::item:selected{ background:#C0F0C0;}"
        "QMenu::separator{ height: 1px; background: gray; margin-left: 2px; margin-right: 2px;}");

    if (pItem)
    {
        switch (GetItemType(pItem))
        {
        case bnb::credential_enum::ct_credential:
            treeMenu.addAction(_ui.m_actEditCredential);
            treeMenu.addAction(_ui.m_actModifyPassword);
            treeMenu.addSeparator();
            treeMenu.addAction(_ui.m_actAddPlatform);
            break;
        case bnb::credential_enum::ct_property:
            treeMenu.addAction(_ui.m_actEditProperty);
            treeMenu.addAction(_ui.m_actDelProperty);
            break;
        case bnb::credential_enum::ct_account:
            treeMenu.addAction(_ui.m_actEditAccount);
            treeMenu.addAction(_ui.m_actDelAccount);
            treeMenu.addSeparator();
            treeMenu.addAction(_ui.m_actAddProperty);
            break;
        case bnb::credential_enum::ct_platform:
            treeMenu.addAction(_ui.m_actEditPlatform);
            treeMenu.addAction(_ui.m_actDelPlatform);
            treeMenu.addSeparator();
            treeMenu.addAction(_ui.m_actAddAccount);
            break;
        default:
            break;
        }
    }

    treeMenu.exec(QCursor::pos());
}

void TreeView::OnItemChanged(QTreeWidgetItem * cur, QTreeWidgetItem * pre)
{
    if (cur)
    {
        switch (GetItemType(cur))
        {
        case bnb::credential_enum::ct_credential:
            break;
        case bnb::credential_enum::ct_platform:
            break;
        case bnb::credential_enum::ct_property:
        {
            if (auto item_account = cur->parent())
                if (bnb::credential_enum::ct_account == GetItemType(item_account))
                    cur = item_account;

            break;
        }
        case bnb::credential_enum::ct_account:
            break;
        default:
            return;
        }

        if (_delegate)
            _delegate->SwitchNode(GetItemID(cur));
    }
}

void TreeView::OnDoubleClickedItem(QTreeWidgetItem * pItem, int index)
{
    std::cout << "index: " << index << ", text: " << pItem->text(0).toStdString() << std::endl;

    if (0 == index && pItem)
    {
        switch (GetItemType(*pItem))
        {
        case bnb::credential_enum::ct_credential:
        {
            if (EditCredential(pItem))
                return;

            break;
        }
        case bnb::credential_enum::ct_platform:
        {
            if (EditPlatform(pItem))
                return;

            break;
        }
        case bnb::credential_enum::ct_property:
        {
            if (EditProperty(pItem))
                return;

            break;
        }
        case bnb::credential_enum::ct_account:
        {
            if (EditAccount(pItem))
                return;

            break;
        }
        default:
            break;
        }
    }

    HintDialog(hint_type::ht_warning, "The parameter error !", this).exec();
}

void TreeView::mouseDoubleClickEvent(QMouseEvent * event)
{
    std::cout << "button: " << event->button() << std::endl;

    if (auto pItem = itemAt(event->pos()))
    {
        std::cout << "text: " << pItem->text(0).toStdString() << std::endl;
    }

    std::cout << "---------------------" << std::endl;

    event->accept();
}

QT_END_NAMESPACE
