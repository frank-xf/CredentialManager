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
	// m_treeView->setHeaderHidden(true);

	QTreeWidgetItem* item_root = new QTreeWidgetItem;
	item_root->setSizeHint(0, { ui_utils::tree_item_w, ui_utils::tree_item_h });
	item_root->setData(0, Qt::UserRole, static_cast<unsigned char>(bnb::credential_type::ct_credential));
	setHeaderItem(item_root);
}

void TreeView::InitCredential()
{
	UpdateHeader();

	for (auto ptr_platform = g_AppMgr.Model().Info().Tree().Head(); ptr_platform; ptr_platform = ptr_platform->m_Next)
	{
		QTreeWidgetItem* item_platform = AddPlatform(&ptr_platform->m_Pair.m_Key);
		for (auto ptr_account = ptr_platform->m_Pair.m_Value.Head(); ptr_account; ptr_account = ptr_account->m_Next)
		{
			QTreeWidgetItem* item_account = AddAccount(item_platform, &ptr_account->m_Pair.m_Key);
			for (auto ptr_property = ptr_account->m_Pair.m_Value.Head(); ptr_property; ptr_property = ptr_property->m_Next)
			{
				QTreeWidgetItem* item_property = AddProperty(item_account, &ptr_property->m_Pair.m_Key);
			}
		}
	}

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

QTreeWidgetItem * TreeView::AddPlatform(const bnb::platform_type * pp)
{
	if (pp)
	{
		QTreeWidgetItem* item_platform = new QTreeWidgetItem(this, { QString::fromStdString(pp->m_strName) });
		item_platform->setForeground(0, QBrush(QColor(216, 32, 32)));
		item_platform->setSizeHint(0, { ui_utils::tree_item_w, ui_utils::tree_item_h });
		item_platform->setData(0, Qt::UserRole, static_cast<unsigned char>(bnb::credential_type::ct_platform));
		addTopLevelItem(item_platform);

		return item_platform;
	}

	return nullptr;
}

QTreeWidgetItem* TreeView::AddAccount(QTreeWidgetItem* parent, const bnb::account_type* pa)
{
	if (pa)
	{
		QTreeWidgetItem* item_account = new QTreeWidgetItem(parent, { QString::fromStdString(pa->m_strName) });
		item_account->setForeground(0, QBrush(QColor(64, 128, 255)));
		item_account->setSizeHint(0, { ui_utils::tree_item_w, ui_utils::tree_item_h });
		item_account->setData(0, Qt::UserRole, static_cast<unsigned char>(bnb::credential_type::ct_account));
		parent->addChild(item_account);

		return item_account;
	}

	return nullptr;
}

QTreeWidgetItem * TreeView::AddProperty(QTreeWidgetItem * parent, const bnb::property_key * pp)
{
	if (pp)
	{
		QTreeWidgetItem* item_property = new QTreeWidgetItem(parent, { QString::fromStdString(pp->m_strName) });
		item_property->setForeground(0, QBrush(QColor(32, 192, 32)));
		item_property->setSizeHint(0, { ui_utils::tree_item_w, ui_utils::tree_item_h });
		item_property->setData(0, Qt::UserRole, static_cast<unsigned char>(bnb::credential_type::ct_property));
		parent->addChild(item_property);

		return item_property;
	}

	return nullptr;
}
