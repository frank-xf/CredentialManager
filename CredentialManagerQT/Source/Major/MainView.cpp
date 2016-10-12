#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QMenu>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStyleFactory>

#include "credential_qt_utils.h"
#include "credential_qt_manager.h"
#include "credential_model_manager.h"

#include "Major/ToolBar.h"
#include "Major/ContentView.h"

#include "Major/MainView.h"
#include "Major/CredentialItem.h"
#include "Major/CredentialView.h"
#include "Major/CredentialDialog.h"

#include "Dialog/HintDialog.h"
#include "Dialog/PasswordInput.h"
#include "Dialog/CreateDialog.h"
#include "Dialog/EditDialog.h"

QT_BEGIN_NAMESPACE

inline static bnb::credential_type GetItemType(const QTreeWidgetItem& item)
{
    return static_cast<bnb::credential_type>(item.data(0, Qt::UserRole).toUInt());
}

MainView::MainView(QWidget *parent)
    : QWidget(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint)
{
	setAttribute(Qt::WA_DeleteOnClose, true);

    ui_utils::SetBackgroundColor(this, ui_utils::g_clrManView);

    _ui.SetupUI(this);

    QObject::connect(_ui.m_viewToolBar->UI().m_btnNew, &QPushButton::clicked, this, &MainView::OnClickedNew);
    QObject::connect(_ui.m_viewToolBar->UI().m_btnOpen, &QPushButton::clicked, this, &MainView::OnClickedOpen);
    QObject::connect(_ui.m_viewToolBar->UI().m_btnAbout, &QPushButton::clicked, this, &MainView::OnClickedAbout);

    QObject::connect(_ui.m_treeView, &QTreeWidget::customContextMenuRequested, this, &MainView::OnTreeContextMenu);
    QObject::connect(_ui.m_treeView, &QTreeWidget::currentItemChanged, this, &MainView::OnItemChanged);
	QObject::connect(_ui.m_treeView, &QTreeWidget::itemDoubleClicked, this, &MainView::OnDoubleClickedItem);

	QObject::connect(_ui.m_actAddAccount, &QAction::triggered, this, &MainView::OnAddAccount);
	QObject::connect(_ui.m_actAddPlatform, &QAction::triggered, this, &MainView::OnAddPlatform);
	QObject::connect(_ui.m_actAddProperty, &QAction::triggered, this, &MainView::OnAddProperty);
	QObject::connect(_ui.m_actDelAccount, &QAction::triggered, this, &MainView::OnRemoveAccount);
	QObject::connect(_ui.m_actDelPlatform, &QAction::triggered, this, &MainView::OnRemovePlatform);
	QObject::connect(_ui.m_actDelProperty, &QAction::triggered, this, &MainView::OnRemoveProperty);
	QObject::connect(_ui.m_actEditAccount, &QAction::triggered, this, &MainView::OnEditAccount);
	QObject::connect(_ui.m_actEditPlatform, &QAction::triggered, this, &MainView::OnEditPlatform);
	QObject::connect(_ui.m_actEditProperty, &QAction::triggered, this, &MainView::OnEditProperty);
	QObject::connect(_ui.m_actModifyPassword, &QAction::triggered, this, &MainView::OnMotifyPassword);
	QObject::connect(_ui.m_actEditCredential, &QAction::triggered, this, &MainView::OnEditCredential);

    InitCredential();
}

void MainView::InitCredential()
{
    QTreeWidgetItem* item_root = new QTreeWidgetItem;
    item_root->setSizeHint(0, { ui_utils::tree_item_w, ui_utils::tree_item_h });
    item_root->setData(0, Qt::UserRole, static_cast<unsigned char>(bnb::credential_type::ct_credential));
    _ui.m_treeView->setHeaderItem(item_root);

	UpdateHeader();

    for (auto ptr_platform = g_AppMgr.Model().Info().Tree().Head(); ptr_platform; ptr_platform = ptr_platform->m_Next)
    {
        QTreeWidgetItem* item_platform = new QTreeWidgetItem(_ui.m_treeView, { QString::fromStdString(ptr_platform->m_Pair.m_Key.m_strName) });
        item_platform->setSizeHint(0, { ui_utils::tree_item_w, ui_utils::tree_item_h });
        item_platform->setData(0, Qt::UserRole, static_cast<unsigned char>(bnb::credential_type::ct_platform));
        _ui.m_treeView->addTopLevelItem(item_platform);

        for (auto ptr_account = ptr_platform->m_Pair.m_Value.Head(); ptr_account; ptr_account = ptr_account->m_Next)
        {
            QTreeWidgetItem* item_account = new QTreeWidgetItem(item_platform, { QString::fromStdString(ptr_account->m_Pair.m_Key.m_strName) });
            item_account->setSizeHint(0, { ui_utils::tree_item_w, ui_utils::tree_item_h });
            item_account->setData(0, Qt::UserRole, static_cast<unsigned char>(bnb::credential_type::ct_account));
            item_platform->addChild(item_account);
            
            for (auto ptr_property = ptr_account->m_Pair.m_Value.Head(); ptr_property; ptr_property = ptr_property->m_Next)
            {
                QTreeWidgetItem* item_property = new QTreeWidgetItem(item_account, { QString::fromStdString(ptr_property->m_Pair.m_Key.m_strName) });
                item_property->setSizeHint(0, { ui_utils::tree_item_w, ui_utils::tree_item_h });
                item_property->setData(0, Qt::UserRole, static_cast<unsigned char>(bnb::credential_type::ct_property));
                item_account->addChild(item_property);
            }
        }
    }

    _ui.m_treeView->expandAll();
}

void MainView::ClearCredential()
{
	_ui.m_treeView->clear();

}

void MainView::UpdateHeader()
{
	QTreeWidgetItem* pItem = _ui.m_treeView->headerItem();
	if (pItem)
	{
		if (g_AppMgr.Model().Info().IsValid())
			pItem->setText(0, QString::fromStdString("Credential - " + g_AppMgr.Model().Info().GetUser()));
		else
			pItem->setText(0, "Credential");
	}
}

void MainView::OnClickedNew()
{
	CreateDialog dlg(this);

	if (QDialog::Accepted == dlg.exec())
	{
		ClearCredential();
		InitCredential();
	}
}

void MainView::OnClickedOpen()
{
	/*
	QString strFile = QFileDialog::getOpenFileName(
	this, "Please select a credential file", ".", "credential file(*.credential)");
	*/

	QString strFile("def.credential");
	if (strFile.isEmpty()) return;

	if (g_AppMgr.Model().Load(strFile.toStdString()))
	{
		ClearCredential();
		InitCredential();
	}
}

void MainView::OnClickedAbout()
{
}

void MainView::OnTreeContextMenu(const QPoint & pos)
{
	QTreeWidgetItem* pItem = _ui.m_treeView->itemAt(pos);

	QMenu treeMenu(_ui.m_treeView);

	if (nullptr == pItem /*|| nullptr == pItem->parent()*/)
	{
		treeMenu.addAction(_ui.m_actEditCredential);
		treeMenu.addAction(_ui.m_actModifyPassword);
		treeMenu.addSeparator();
		treeMenu.addAction(_ui.m_actAddPlatform);
	}
	else
	{
		switch (static_cast<bnb::credential_type>(pItem->data(0, Qt::UserRole).toUInt()))
		{
		case bnb::credential_type::ct_property:
			treeMenu.addAction(_ui.m_actEditProperty);
			treeMenu.addAction(_ui.m_actDelProperty);
			break;
		case bnb::credential_type::ct_account:
			treeMenu.addAction(_ui.m_actEditAccount);
			treeMenu.addAction(_ui.m_actDelAccount);
			treeMenu.addSeparator();
			treeMenu.addAction(_ui.m_actAddProperty);
			break;
		case bnb::credential_type::ct_platform:
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

void MainView::OnItemChanged(QTreeWidgetItem * cur, QTreeWidgetItem * pre)
{
}

void MainView::OnDoubleClickedItem(QTreeWidgetItem * pItem, int index)
{
}

void MainView::OnEditCredential()
{
	EditCredentialDialog dlg(this);

	if (QDialog::Accepted == dlg.exec())
	{
		UpdateHeader();
	}
}

void MainView::OnMotifyPassword()
{
	EditPasswordDialog dlg(this);

	dlg.exec();
}

void MainView::OnAddPlatform()
{
}

void MainView::OnAddAccount()
{
}

void MainView::OnAddProperty()
{
}

void MainView::OnEditPlatform()
{
}

void MainView::OnEditAccount()
{
    QTreeWidgetItem* pAccount = _ui.m_treeView->currentItem();
    if (nullptr == pAccount)
    {
        return;
    }
    if (bnb::credential_type::ct_account != GetItemType(*pAccount))
    {
        return;
    }

    QTreeWidgetItem* pPlatform = pAccount->parent();
    if (nullptr == pPlatform)
    {
        return;
    }
    if (bnb::credential_type::ct_platform != GetItemType(*pPlatform))
    {
        return;
    }


}

void MainView::OnEditProperty()
{
}

void MainView::OnRemovePlatform()
{
}

void MainView::OnRemoveAccount()
{
}

void MainView::OnRemoveProperty()
{
}

void MainView::ui_type::SetupUI(MainView* pView)
{
    pView->setObjectName("MainView");
    pView->setWindowTitle("Credential Manager");

    QSplitter* phSplitter = new QSplitter(Qt::Horizontal, pView);
    phSplitter->setObjectName("MainSplitter");
    phSplitter->setHandleWidth(1);
    phSplitter->setOpaqueResize(true);
    phSplitter->setChildrenCollapsible(false);
    phSplitter->setStretchFactor(1, 1);

    m_treeView = new QTreeWidget(phSplitter);
    m_treeView->setStyle(QStyleFactory::create("Windows"));
    m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    // m_treeView->setHeaderHidden(true);

    m_viewContent = new ContentView(phSplitter);
    
    m_viewToolBar = new ToolBar(pView);

    QVBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->setMargin(2);
    pMainLayout->setSpacing(2);
    pMainLayout->addWidget(m_viewToolBar);
    pMainLayout->addWidget(phSplitter, 1);

    pView->setLayout(pMainLayout);

    m_actAddAccount = new QAction(m_treeView);
    m_actAddPlatform = new QAction(m_treeView);
    m_actAddProperty = new QAction(m_treeView);
    m_actDelAccount = new QAction(m_treeView);
    m_actDelPlatform = new QAction(m_treeView);
    m_actDelProperty = new QAction(m_treeView);
    m_actEditAccount = new QAction(m_treeView);
    m_actEditPlatform = new QAction(m_treeView);
    m_actEditProperty = new QAction(m_treeView);
    m_actModifyPassword = new QAction(m_treeView);
    m_actEditCredential = new QAction(m_treeView);

    RetranslateUI(pView);
}

void MainView::ui_type::RetranslateUI(MainView * pView)
{
    m_actAddAccount->setText("Add Account");
    m_actAddPlatform->setText("Add Platform");
    m_actAddProperty->setText("Add Property");
    m_actDelAccount->setText("Remove Account");
    m_actDelPlatform->setText("Remove Platform");
    m_actDelProperty->setText("Remove Property");
    m_actEditAccount->setText("Edit Account");
    m_actEditPlatform->setText("Edit Platform");
    m_actEditProperty->setText("Edit Property");
    m_actModifyPassword->setText("Modify Password");
    m_actEditCredential->setText("Edit Credential");

}

QT_END_NAMESPACE
