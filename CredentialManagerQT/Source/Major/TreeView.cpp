#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QStyleFactory>

#include "credential_qt_utils.h"
#include "credential_qt_manager.h"
#include "credential_model_manager.h"

#include "Major/TreeView.h"

static inline void SetTreeItem(QTreeWidgetItem* pItem, bnb::credential_type t, const QColor& c)
{
    pItem->setTextColor(0, c);
    pItem->setSizeHint(0, { ui_utils::tree_item_w, ui_utils::tree_item_h });
    pItem->setData(0, Qt::UserRole, static_cast<unsigned char>(t));

    QFont font;
    font.setPointSize(10);
    pItem->setFont(0, font);
}

static inline QTreeWidgetItem* MakeTreeItem(QTreeWidgetItem* p, const QString& strText, bnb::credential_type t, const QColor& c)
{
    QTreeWidgetItem* pItem = new QTreeWidgetItem(p, { strText });
    SetTreeItem(pItem, t, c);

    return pItem;
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
        "QTreeView::item{ border: none; }\n"
        "QTreeView::item:hover{ background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #B0FFB0, stop:1 #F0FFF0); }\n"
        "QTreeView::item:selected{ background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #2080FF, stop:1 #F0FFF0); }\n"
        "QTreeView::item:selected:active{ background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #2080FF, stop:1 #F0FFF0); color:white; }\n"
        "QTreeView::item:selected:!active{ background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #2080FF, stop:1 #F0FFF0); color:white; }");

}

void TreeView::InitCredential()
{
    QTreeWidgetItem* item_root = new QTreeWidgetItem(this, { QString::fromStdString("Credential - " + g_AppMgr.Model().Info().GetUser()) });
    SetTreeItem(item_root, bnb::credential_type::ct_credential, { 0, 128, 0 });
    addTopLevelItem(item_root);

	g_AppMgr.Model().Info().Tree().Foreach([this, item_root](const bnb::platform_tree::data_type& platform) {
		QTreeWidgetItem* item_platform = AddPlatform(item_root, platform);

		platform.m_Value.Foreach([this, item_platform](const bnb::account_tree::data_type& account) {
			QTreeWidgetItem* item_account = AddAccount(item_platform, account);

			account.m_Value.Foreach([this, item_account](const bnb::property_tree::data_type& property) {
				QTreeWidgetItem* item_property = AddProperty(item_account, property);
			
				return true;
			});

			return true;
		});

		return true;
	});

	expandAll();
}

void TreeView::ClearCredential()
{
	clear();
}

void TreeView::UpdateHeader()
{
    QTreeWidgetItem* pItem = topLevelItem(0);

	if (pItem)
	{
		if (g_AppMgr.Model().Info().IsValid())
			pItem->setText(0, QString::fromStdString("Credential - " + g_AppMgr.Model().Info().GetUser()));
		else
			pItem->setText(0, "Credential");
	}
}

QTreeWidgetItem * TreeView::AddPlatform(QTreeWidgetItem* parent, const bnb::platform_tree::data_type& pp)
{
    auto item_platform = MakeTreeItem(parent, QString::fromStdString(pp.m_Key.m_strName), bnb::credential_type::ct_platform, { 216, 32, 32 });
    parent->addChild(item_platform);

	return item_platform;
}

QTreeWidgetItem* TreeView::AddAccount(QTreeWidgetItem* parent, const bnb::account_tree::data_type& pa)
{
    auto item_account = MakeTreeItem(parent, QString::fromStdString(pa.m_Key.m_strName), bnb::credential_type::ct_account, { 64, 128, 255 });
	parent->addChild(item_account);

	return item_account;
}

QTreeWidgetItem * TreeView::AddProperty(QTreeWidgetItem * parent, const bnb::property_tree::data_type& pp)
{
    auto item_property = MakeTreeItem(parent, QString::fromStdString(pp.m_Key.m_strName), bnb::credential_type::ct_property, { 32, 192, 32 });
	parent->addChild(item_property);

	return item_property;
}
