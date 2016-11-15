#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QStyleFactory>

#include "credential_qt_utils.h"
#include "credential_qt_manager.h"
#include "credential_model_manager.h"

#include "Major/TreeView.h"

TreeView::TreeView(QWidget * parent) : QTreeWidget(parent)
{
	setStyle(QStyleFactory::create("Windows"));
	setContextMenuPolicy(Qt::CustomContextMenu);

	QTreeWidgetItem* item_root = new QTreeWidgetItem;
	item_root->setSizeHint(0, { ui_utils::tree_item_w, ui_utils::tree_item_h });
	item_root->setData(0, Qt::UserRole, static_cast<unsigned char>(bnb::credential_type::ct_credential));
	setHeaderItem(item_root);
}

void TreeView::InitCredential()
{
	UpdateHeader();

	g_AppMgr.Model().Info().Tree().Foreach([this](const bnb::platform_tree::data_type& platform) {
		QTreeWidgetItem* item_platform = AddPlatform(platform);

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
	QTreeWidgetItem* pItem = headerItem();
	if (pItem)
	{
		if (g_AppMgr.Model().Info().IsValid())
			pItem->setText(0, QString::fromStdString("Credential - " + g_AppMgr.Model().Info().GetUser()));
		else
			pItem->setText(0, "Credential");
	}
}

QTreeWidgetItem * TreeView::AddPlatform(const bnb::platform_tree::data_type& pp)
{
	QTreeWidgetItem* item_platform = new QTreeWidgetItem(this, { QString::fromStdString(pp.m_Key.m_strName) });
	item_platform->setForeground(0, QBrush(QColor(216, 32, 32)));
	item_platform->setSizeHint(0, { ui_utils::tree_item_w, ui_utils::tree_item_h });
	item_platform->setData(0, Qt::UserRole, static_cast<unsigned char>(bnb::credential_type::ct_platform));
	addTopLevelItem(item_platform);

	return item_platform;
}

QTreeWidgetItem* TreeView::AddAccount(QTreeWidgetItem* parent, const bnb::account_tree::data_type& pa)
{
	QTreeWidgetItem* item_account = new QTreeWidgetItem(parent, { QString::fromStdString(pa.m_Key.m_strName) });
	item_account->setForeground(0, QBrush(QColor(64, 128, 255)));
	item_account->setSizeHint(0, { ui_utils::tree_item_w, ui_utils::tree_item_h });
	item_account->setData(0, Qt::UserRole, static_cast<unsigned char>(bnb::credential_type::ct_account));
	parent->addChild(item_account);

	return item_account;
}

QTreeWidgetItem * TreeView::AddProperty(QTreeWidgetItem * parent, const bnb::property_tree::data_type& pp)
{
	QTreeWidgetItem* item_property = new QTreeWidgetItem(parent, { QString::fromStdString(pp.m_Key.m_strName) });
	item_property->setForeground(0, QBrush(QColor(32, 192, 32)));
	item_property->setSizeHint(0, { ui_utils::tree_item_w, ui_utils::tree_item_h });
	item_property->setData(0, Qt::UserRole, static_cast<unsigned char>(bnb::credential_type::ct_property));
	parent->addChild(item_property);

	return item_property;
}
