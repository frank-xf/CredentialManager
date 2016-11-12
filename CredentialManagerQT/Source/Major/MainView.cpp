#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QMenu>
#include <QtWidgets/QSplitter>

#include "credential_qt_utils.h"
#include "credential_qt_manager.h"
#include "credential_model_manager.h"

#include "Dialog/HintDialog.h"
#include "Dialog/PasswordInput.h"
#include "Dialog/CreateDialog.h"
#include "Dialog/EditDialog.h"

#include "Major/ToolBar.h"
#include "Major/ContentView.h"
#include "Major/TreeView.h"
#include "Major/MainView.h"

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
	_ui.m_treeView->InitCredential();
	_ui.m_viewContent->InitCredential();
}

void MainView::ClearCredential()
{
	_ui.m_treeView->ClearCredential();
	_ui.m_viewContent->ClearCredential();
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
	if (cur)
	{
		switch (GetItemType(*cur))
		{
		case bnb::credential_type::ct_credential:
		{
			if (g_AppMgr.Model().Info().IsValid())
				_ui.m_viewContent->SwitchToCredential(g_AppMgr.Model().Info().GetID());
			else
				_ui.m_viewContent->SwitchToHint();

			return;
		}
		case bnb::credential_type::ct_platform:
		{
			if (auto ptr_platform = g_AppMgr.Model().FindPlatform({ cur->text(0).toStdString() }))
				_ui.m_viewContent->SwitchToPlatform(ptr_platform->m_Key.m_ID);

			return;
		}
		case bnb::credential_type::ct_property:
		{
			auto pAccount = cur->parent();
			if (pAccount && bnb::credential_type::ct_account == GetItemType(*pAccount))
				cur = pAccount;
			else
				return;
		}
		case bnb::credential_type::ct_account:
		{
			auto pPlatform = cur->parent();
			if (pPlatform && bnb::credential_type::ct_platform == GetItemType(*pPlatform))
				if (auto ptr_account = g_AppMgr.Model().FindAccount(pPlatform->text(0).toStdString(), cur->text(0).toStdString()))
					_ui.m_viewContent->SwitchToAccount(ptr_account->m_Key.m_ID);

			return;
		}
		default:
			break;
		}
	}
}

void MainView::OnDoubleClickedItem(QTreeWidgetItem * pItem, int index)
{
	if (0 == index)
	{
		switch (GetItemType(*pItem))
		{
		case bnb::credential_type::ct_credential:
		{
			Edi

			break;
		}
		case bnb::credential_type::ct_platform:
		{
			if (auto ptr_platform = g_AppMgr.Model().FindPlatform({ cur->text(0).toStdString() }))
				_ui.m_viewContent->SwitchToPlatform(ptr_platform->m_Key.m_ID);

			return;
		}
		case bnb::credential_type::ct_property:
		{
			auto pAccount = cur->parent();
			if (pAccount && bnb::credential_type::ct_account == GetItemType(*pAccount))
				cur = pAccount;
			else
				return;
		}
		case bnb::credential_type::ct_account:
		{
			auto pPlatform = cur->parent();
			if (pPlatform && bnb::credential_type::ct_platform == GetItemType(*pPlatform))
				if (auto ptr_account = g_AppMgr.Model().FindAccount(pPlatform->text(0).toStdString(), cur->text(0).toStdString()))
					_ui.m_viewContent->SwitchToAccount(ptr_account->m_Key.m_ID);

			return;
		}
		default:
			break;
		}
	}
}

void MainView::OnEditCredential()
{
	EditCredentialDialog dlg(g_AppMgr.Model().Info(), this);

	if (QDialog::Accepted == dlg.exec())
	{
		g_AppMgr.Model().SaveCredential();

		_ui.m_treeView->UpdateHeader();
		_ui.m_viewContent->UpdateCredential(g_AppMgr.Model().Info().GetID());
	}
}

void MainView::OnMotifyPassword()
{
	EditPasswordDialog dlg(g_AppMgr.Model().Info(), this);

	if (QDialog::Accepted == dlg.exec())
	{
		g_AppMgr.Model().SaveCredential();
	}
}

void MainView::OnAddPlatform()
{
	_ui.m_treeView->AddPlatform
}

void MainView::OnAddAccount()
{
	QTreeWidgetItem* pPlatform = _ui.m_treeView->currentItem();
	if (pPlatform && bnb::credential_type::ct_platform != GetItemType(*pPlatform))
	{
		auto ptr_platform = g_AppMgr.Model().FindPlatform({ pPlatform->text(0).toStdString() });
		if (ptr_platform)
		{


			EditAccountDialog dlg(*ptr_platform, nullptr, this);
			if (QDialog::Accepted == dlg.exec())
			{
				_ui.m_treeView->AddAccount(pPlatform, &dlg.GetAccount()->m_Key);


				/*

				...

				*/
			}
		}
	}
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
    if (pAccount && bnb::credential_type::ct_account == GetItemType(*pAccount))
    {
		QTreeWidgetItem* pPlatform = pAccount->parent();
		if (pPlatform && bnb::credential_type::ct_platform == GetItemType(*pPlatform))
		{
			if (auto ptr_platform = g_AppMgr.Model().FindPlatform({ pPlatform->text(0).toStdString() }))
			{
				if (auto ptr_account = ptr_platform->m_Value.Find({ pAccount->text(0).toStdString() }))
				{
					EditAccountDialog dlg(*ptr_platform, ptr_account, this);
					if (QDialog::Accepted == dlg.exec())
					{
						pAccount->setText(0, QString::fromStdString(ptr_account->m_Key.m_strName));
						_ui.m_viewContent->UpdateAccount(ptr_platform->m_Key.m_ID, ptr_account->m_Key.m_ID);
					}

					return;
				}
			}
		}
    }

	HintDialog(hint_type::ht_warning, "The parameter error !", this).exec();
}

void MainView::OnEditProperty()
{
}

void MainView::OnRemovePlatform()
{
	QTreeWidgetItem* pPlatform = _ui.m_treeView->currentItem();
	if (nullptr == pPlatform || bnb::credential_type::ct_platform != GetItemType(*pPlatform))
	{
		return;
	}

	auto ptr_platform = g_AppMgr.Model().Info().Tree().Find({ pPlatform->text(0).toStdString() });
	if (ptr_platform)
	{
		std::vector<unsigned int> vtrIds{ ptr_platform->m_Key.m_ID };
		for (auto ptr_account = ptr_platform->m_Value.Head(); ptr_account; ptr_account = ptr_account->m_Next)
			vtrIds.push_back(ptr_account->m_Pair.m_Key.m_ID);

		if (g_AppMgr.Model().Info().Tree().Remove(ptr_platform->m_Key))
		{
			auto ptr_parent = pPlatform->parent();
			if (ptr_parent)
				ptr_parent->removeChild(pPlatform);
			else
				_ui.m_treeView->takeTopLevelItem(_ui.m_treeView->indexOfTopLevelItem(pPlatform));

			delete pPlatform;

			_ui.m_viewContent->RemovePlatform(ptr_platform->m_Key.m_ID, vtrIds);
		}
	}
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

    m_treeView = new TreeView(phSplitter);

    m_viewContent = new ContentView(phSplitter);
    
    m_viewToolBar = new ToolBar(pView);

    QVBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->setMargin(2);
    pMainLayout->setSpacing(2);
    pMainLayout->addWidget(m_viewToolBar);
    pMainLayout->addWidget(phSplitter, 1);

    pView->setLayout(pMainLayout);

    m_actAddAccount = new QAction(pView);
    m_actAddPlatform = new QAction(pView);
    m_actAddProperty = new QAction(pView);
    m_actDelAccount = new QAction(pView);
    m_actDelPlatform = new QAction(pView);
    m_actDelProperty = new QAction(pView);
    m_actEditAccount = new QAction(pView);
    m_actEditPlatform = new QAction(pView);
    m_actEditProperty = new QAction(pView);
    m_actModifyPassword = new QAction(pView);
    m_actEditCredential = new QAction(pView);

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
