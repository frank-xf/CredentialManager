#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtCore/QDateTime>

#include "credential_qt_utils.h"

#include "Credential/Credential.h"

#include "Widget/ContentView.h"

//==============================================================================
// Implementation of CredentialView
//==============================================================================
CredentialView::CredentialView(bnb::Credential& credential, QWidget * parent)
	: base_type(credential.GetID(), parent)
	, m_Credential(credential)
{
	UpdateInfo();
	UpdateTable();
}

void CredentialView::UpdateInfo()
{
	_ui.m_labText[0]->setText(QDateTime::fromTime_t(m_Credential.GetTime()).toString("yyyy-MM-dd HH:mm:ss"));
	_ui.m_labText[1]->setText(QString::fromStdString(m_Credential.GetUser()));
	_ui.m_labText[2]->setText("-");
}

void CredentialView::UpdateTable()
{
	_ui.m_tabView->clearContents();

	unsigned int nRows = m_Credential.Tree().Size();
	_ui.m_tabView->setRowCount(nRows);
	_ui.m_tabView->setEnabled(0 < nRows);

	if (0 < nRows)
	{
		unsigned int nIndex = 0;
		for (auto ptr = m_Credential.Tree().Head(); ptr; ptr = ptr->m_Next)
		{
			QTableWidgetItem* pName = new QTableWidgetItem(QString::fromStdString(ptr->m_Pair.m_Key.m_strName));
			pName->setData(Qt::UserRole, ptr->m_Pair.m_Key.m_ID);
			QTableWidgetItem* pUrl = new QTableWidgetItem(QString::fromStdString(ptr->m_Pair.m_Key.m_strUrl));
			pUrl->setData(Qt::UserRole, ptr->m_Pair.m_Key.m_ID);
			QTableWidgetItem* pDisplay = new QTableWidgetItem(QString::fromStdString(ptr->m_Pair.m_Key.m_strDisplay));
			pDisplay->setData(Qt::UserRole, ptr->m_Pair.m_Key.m_ID);

			_ui.m_tabView->setItem(nIndex, 0, pName);
			_ui.m_tabView->setItem(nIndex, 1, pUrl);
			_ui.m_tabView->setItem(nIndex, 2, pDisplay);

			++nIndex;
		}

		_ui.m_tabView->resizeColumnsToContents();
	}
}

void CredentialView::UpdateTable(unsigned int id)
{
	for (int i = 0; i < _ui.m_tabView->rowCount(); ++i)
	{
		if (auto pItem = _ui.m_tabView->item(i, 0))
		{
			if (id == pItem->data(Qt::UserRole).toUInt())
			{
				for (auto ptr = m_Credential.Tree().Head(); ptr; ptr = ptr->m_Next)
				{
					if (ptr->m_Pair.m_Key.m_ID == id)
					{
						pItem->setText(QString::fromStdString(ptr->m_Pair.m_Key.m_strName));
						_ui.m_tabView->item(i, 1)->setText(QString::fromStdString(ptr->m_Pair.m_Key.m_strUrl));
						_ui.m_tabView->item(i, 2)->setText(QString::fromStdString(ptr->m_Pair.m_Key.m_strDisplay));
						break;
					}
				}

				break;
			}
		}
	}
}

//------------------------------------------------------------------------------

template<>
void CredentialView::base_type::ui_type::RetranslateUI(QWidget* pView)
{
	m_tabView->setColumnCount(3);
	m_tabView->setHorizontalHeaderLabels({ "    Platform    ", "    Url    ", "    Display    " });

	_labText[0]->setText("Update Time: ");
	_labText[1]->setText("User Name: ");
	_labText[2]->setText("Display: ");
}

//==============================================================================
// Implementation of PlatformView
//==============================================================================
PlatformView::PlatformView(const bnb::platform_tree::data_type& tp, QWidget * parent)
	: base_type(tp.m_Key.m_ID, parent)
	, m_Platform(tp)
{
	UpdateInfo();
	UpdateTable();
}

void PlatformView::UpdateInfo()
{
	_ui.m_labText[0]->setText(QString::fromStdString(m_Platform.m_Key.m_strName));
	_ui.m_labText[1]->setText(QString::fromStdString(m_Platform.m_Key.m_strUrl));
	_ui.m_labText[2]->setText(QString::fromStdString(m_Platform.m_Key.m_strDisplay));
}

void PlatformView::UpdateTable()
{
	_ui.m_tabView->clearContents();

	unsigned int nRows = m_Platform.m_Value.Size();
	_ui.m_tabView->setRowCount(nRows);
	_ui.m_tabView->setEnabled(0 < nRows);

	if (0 < nRows)
	{
		unsigned int nIndex = 0;
		for (auto ptr = m_Platform.m_Value.Head(); ptr; ptr = ptr->m_Next)
		{
			QTableWidgetItem* pName = new QTableWidgetItem(QString::fromStdString(ptr->m_Pair.m_Key.m_strName));
			pName->setData(Qt::UserRole, ptr->m_Pair.m_Key.m_ID);
			QTableWidgetItem* pDisplay = new QTableWidgetItem(QString::fromStdString(ptr->m_Pair.m_Key.m_strDisplay));
			pDisplay->setData(Qt::UserRole, ptr->m_Pair.m_Key.m_ID);

			_ui.m_tabView->setItem(nIndex, 0, pName);
			_ui.m_tabView->setItem(nIndex, 1, pDisplay);

			++nIndex;
		}

		_ui.m_tabView->resizeColumnsToContents();
	}
}

void PlatformView::UpdateTable(unsigned int id)
{
	for (int i = 0; i < _ui.m_tabView->rowCount(); ++i)
	{
		if (auto pItem = _ui.m_tabView->item(i, 0))
		{
			if (id == pItem->data(Qt::UserRole).toUInt())
			{
				for (auto ptr = m_Platform.m_Value.Head(); ptr; ptr = ptr->m_Next)
				{
					if (ptr->m_Pair.m_Key.m_ID == id)
					{
						pItem->setText(QString::fromStdString(ptr->m_Pair.m_Key.m_strName));
						_ui.m_tabView->item(i, 1)->setText(QString::fromStdString(ptr->m_Pair.m_Key.m_strDisplay));
						break;
					}
				}

				break;
			}
		}
	}
}

//------------------------------------------------------------------------------

template<>
void PlatformView::base_type::ui_type::RetranslateUI(QWidget* pView)
{
	m_tabView->setColumnCount(3);
	m_tabView->setHorizontalHeaderLabels({ "    Account    ", "    Display    " });

	_labText[0]->setText("Platform: ");
	_labText[1]->setText("Url: ");
	_labText[2]->setText("Display: ");
}

//==============================================================================
// Implementation of AccountView
//==============================================================================
AccountView::AccountView(const bnb::account_tree::data_type & tp, QWidget * parent)
	: base_type(tp.m_Key.m_ID, parent)
	, m_Account(tp)
{
	UpdateInfo();
	UpdateTable();
}

void AccountView::UpdateInfo()
{
	_ui.m_labText[0]->setText(QString::fromStdString(m_Account.m_Key.m_strName));
	_ui.m_labText[1]->setText(QString::fromStdString(m_Account.m_Key.m_strDisplay));
}

void AccountView::UpdateTable()
{
	_ui.m_tabView->clearContents();

	unsigned int nRows = m_Account.m_Value.Size();
	_ui.m_tabView->setRowCount(nRows);
	_ui.m_tabView->setEnabled(0 < nRows);

	if (0 < nRows)
	{
		unsigned int nIndex = 0;
		for (auto ptr = m_Account.m_Value.Head(); ptr; ptr = ptr->m_Next)
		{
			QTableWidgetItem* pKey = new QTableWidgetItem(QString::fromStdString(ptr->m_Pair.m_Key.m_strName));
			pKey->setData(Qt::UserRole, ptr->m_Pair.m_Key.m_ID);
			QTableWidgetItem* pValue = new QTableWidgetItem(QString::fromStdString(ptr->m_Pair.m_Value.m_strName));
			pValue->setData(Qt::UserRole, ptr->m_Pair.m_Key.m_ID);

			_ui.m_tabView->setItem(nIndex, 0, pKey);
			_ui.m_tabView->setItem(nIndex, 1, pValue);

			++nIndex;
		}

		_ui.m_tabView->resizeColumnsToContents();
	}
}

void AccountView::UpdateTable(unsigned int id)
{
	for (int i = 0; i < _ui.m_tabView->rowCount(); ++i)
	{
		if (auto pItem = _ui.m_tabView->item(i, 0))
		{
			if (id == pItem->data(Qt::UserRole).toUInt())
			{
				for (auto ptr = m_Account.m_Value.Head(); ptr; ptr = ptr->m_Next)
				{
					if (ptr->m_Pair.m_Key.m_ID == id)
					{
						pItem->setText(QString::fromStdString(ptr->m_Pair.m_Key.m_strName));
						_ui.m_tabView->item(i, 1)->setText(QString::fromStdString(ptr->m_Pair.m_Value.m_strName));
						break;
					}
				}

				break;
			}
		}
	}
}

//------------------------------------------------------------------------------

template<>
void AccountView::base_type::ui_type::RetranslateUI(QWidget* pView)
{
	m_tabView->setColumnCount(2);
	m_tabView->setHorizontalHeaderLabels({ "    Key    ", "    Value    " });

	_labText[0]->setText("Account: ");
	_labText[1]->setText("Display: ");
}
