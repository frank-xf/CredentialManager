#include <QtGui/QMouseEvent>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QStyleFactory>
#include <QtWidgets/QStyledItemDelegate>

#include <iostream>

#include "Credential/Credential.h"

#include "credential_qt_string.h"
#include "credential_qt_utils.h"

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

void TreeView::mouseDoubleClickEvent(QMouseEvent * event)
{
    std::cout<< "button: " <<event->button()<<std::endl;

    if (auto pItem = itemAt(event->pos()))
    {
        std::cout << "text: " << pItem->text(0).toStdString() << std::endl;
    }

    std::cout << "---------------------" << std::endl;

    event->accept();
}

//==============================================================================
// Implementation of TreeView
//==============================================================================
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
            auto item_platform = AddPlatform(item_credential, pp);
            item_credential->setExpanded(true);

            return item_platform;
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
                auto item_account = AddAccount(item_platform, pa);
                item_platform->setExpanded(true);
                
                return item_account;
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
                    auto item_property = AddProperty(item_account, pp);
                    item_property->setExpanded(true);

                    return item_property;
                }
            }
        }
    }

    return nullptr;
}

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

QT_END_NAMESPACE
