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

static inline void SetTreeItem(QTreeWidgetItem* pItem, bnb::credential_type t, const QColor& c)
{
    pItem->setTextColor(0, c);
    pItem->setSizeHint(0, { ui_utils::tree_item_w, ui_utils::def_widget_h });
    pItem->setData(0, Qt::UserRole, static_cast<unsigned char>(t));
    pItem->setFont(0, ui_utils::MakeFont());
}

static inline QTreeWidgetItem* MakeTreeItem(QTreeWidgetItem* p, const QString& strText, bnb::credential_type t, const QColor& c)
{
    QTreeWidgetItem* pItem = new QTreeWidgetItem(p, { strText });
    SetTreeItem(pItem, t, c);

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

QTreeWidgetItem * TreeView::AddRoot(const bnb::Credential & pc)
{
    QTreeWidgetItem* item_root = new QTreeWidgetItem(this, { '[' + To_QString(pc.GetUser()) + ']' });
    SetTreeItem(item_root, bnb::credential_type::ct_credential, ui_utils::g_clrCredential);
    addTopLevelItem(item_root);

    return item_root;
}

QTreeWidgetItem * TreeView::AddPlatform(QTreeWidgetItem* parent, const bnb::platform_list::data_type& pp)
{
    auto item_platform = MakeTreeItem(parent, To_QString(pp.m_Key.m_strName), pp.m_Key.GetType(), ui_utils::g_clrPlatform);
    parent->addChild(item_platform);
    expandItem(parent);

	return item_platform;
}

QTreeWidgetItem* TreeView::AddAccount(QTreeWidgetItem* parent, const bnb::account_list::data_type& pa)
{
    auto item_account = MakeTreeItem(parent, To_QString(pa.m_Key.m_strName), pa.m_Key.GetType(), ui_utils::g_clrAccount);
	parent->addChild(item_account);
    expandItem(parent);

	return item_account;
}

QTreeWidgetItem * TreeView::AddProperty(QTreeWidgetItem * parent, const bnb::property_list::data_type& pp)
{
    auto item_property = MakeTreeItem(parent, To_QString(pp.m_Key.m_strName), pp.m_Key.GetType(), ui_utils::g_clrProperty);
	parent->addChild(item_property);
    expandItem(parent);

	return item_property;
}

QT_END_NAMESPACE
