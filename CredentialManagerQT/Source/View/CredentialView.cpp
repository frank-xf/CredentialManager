#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>
#include <QtCore/QDateTime>

#include "credential_qt_utils.h"

#include "Credential/Credential.h"

#include "View/ViewBase.h"
#include "View/CredentialView.h"

CredentialView::CredentialView(bnb::Credential& credential, QWidget * parent)
	: ViewBase(credential.GetID(), parent)
	, m_Credential(credential)
{
    _ui.SetupUI(this);

	UpdateInfo();
	UpdateTable();
}

void CredentialView::UpdateInfo()
{
	_ui.m_labUpdateTime->setText(QDateTime::fromTime_t(m_Credential.GetTime()).toString("yyyy-MM-dd HH:mm:ss"));
	_ui.m_labUserName->setText(QString::fromStdString(m_Credential.GetUser()));
	_ui.m_labDisplay->setText("-");
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

void CredentialView::ui_type::SetupUI(QWidget* pView)
{
    ViewBase::ui_type::SetupUI(pView);

    m_tabView->setColumnCount(3);

    _labUpdateTime = ui_utils::MakeStaticLabel(pView, 60, 24);
    _labUserName = ui_utils::MakeStaticLabel(pView, 60, 24);
    _labDisplay = ui_utils::MakeStaticLabel(pView, 60, 24);

	m_labUpdateTime = ui_utils::MakeDynamicLabel(pView);
	m_labUserName = ui_utils::MakeDynamicLabel(pView);
	m_labDisplay = ui_utils::MakeDynamicLabel(pView);

	QHBoxLayout* phLayout1 = new QHBoxLayout;
	phLayout1->addWidget(_labUpdateTime);
	phLayout1->addWidget(m_labUpdateTime, 1);
	phLayout1->setMargin(0);
	phLayout1->setSpacing(0);

	QHBoxLayout* phLayout2 = new QHBoxLayout;
	phLayout2->addWidget(_labUserName);
	phLayout2->addWidget(m_labUserName, 1);
	phLayout2->setMargin(0);
	phLayout2->setSpacing(0);

	QHBoxLayout* phLayout3 = new QHBoxLayout;
	phLayout3->addWidget(_labDisplay);
	phLayout3->addWidget(m_labDisplay, 1);
	phLayout3->setMargin(0);
	phLayout3->setSpacing(0);

	QVBoxLayout* pvLayout1 = new QVBoxLayout;
	pvLayout1->addLayout(phLayout1);
	pvLayout1->addLayout(phLayout2);
	pvLayout1->addLayout(phLayout3);
	pvLayout1->setMargin(0);
	pvLayout1->setSpacing(4);

	QVBoxLayout* pvMainLayout = new QVBoxLayout;
	pvMainLayout->addLayout(pvLayout1);
	pvMainLayout->addWidget(m_tabView, 1);
	pvMainLayout->setMargin(10);
	pvMainLayout->setSpacing(10);

	pView->setLayout(pvMainLayout);

    RetranslateUI(pView);
}

void CredentialView::ui_type::RetranslateUI(QWidget* pView)
{
    m_tabView->setHorizontalHeaderLabels({ "Platform", "Url", "Display" });

    _labUpdateTime->setText("Update Time: ");
    _labUserName->setText("User Name: ");
    _labDisplay->setText("Display: ");
}