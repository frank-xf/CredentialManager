#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>

#include "credential_qt_utils.h"

#include "Credential/Credential.h"

#include "View/ViewBase.h"
#include "View/PlatformView.h"

PlatformView::PlatformView(bnb::platform_tree::data_type& tp, QWidget * parent)
	: ViewBase(tp.m_Key.m_ID, parent)
	, m_Platform(tp)
{
	_ui.SetupUI(this);

	UpdateInfo();
	UpdateTable();
}

void PlatformView::UpdateInfo()
{
	_ui.m_labName->setText(QString::fromStdString(m_Platform.m_Key.m_strName));
	_ui.m_labUrl->setText(QString::fromStdString(m_Platform.m_Key.m_strUrl));
	_ui.m_labDisplay->setText(QString::fromStdString(m_Platform.m_Key.m_strDisplay));
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

void PlatformView::ui_type::SetupUI(QWidget* pView)
{
	ViewBase::ui_type::SetupUI(pView);

	m_tabView->setColumnCount(2);

	_labName = ui_utils::MakeStaticLabel(pView, 60, 24);
	_labUrl = ui_utils::MakeStaticLabel(pView, 60, 24);
	_labDisplay = ui_utils::MakeStaticLabel(pView, 60, 24);

	m_labName = ui_utils::MakeDynamicLabel(pView);
	m_labUrl = ui_utils::MakeDynamicLabel(pView);
	m_labDisplay = ui_utils::MakeDynamicLabel(pView);
	
	QHBoxLayout* phLayout1 = new QHBoxLayout;
	phLayout1->addWidget(_labName);
	phLayout1->addWidget(m_labName, 1);
	phLayout1->setMargin(0);
	phLayout1->setSpacing(0);

	QHBoxLayout* phLayout2 = new QHBoxLayout;
	phLayout2->addWidget(_labUrl);
	phLayout2->addWidget(m_labUrl, 1);
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

void PlatformView::ui_type::RetranslateUI(QWidget* pView)
{
	m_tabView->setHorizontalHeaderLabels({ "Account", "Display" });

	_labName->setText("Platform: ");
	_labUrl->setText("Url: ");
	_labDisplay->setText("Display: ");
}
