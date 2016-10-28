#include <QtWidgets/QTableWidget>
#include <QtWidgets/QHeaderView>

#include "View/ViewBase.h"

ViewBase::ViewBase(unsigned int id, QWidget * parent)
	: QWidget(parent)
	, m_ID(id)
{
	
}

void ViewBase::ui_type::SetupUI(QWidget* pView)
{
    m_tabView = new QTableWidget(pView);
    m_tabView->setMinimumSize(240, 64);
    m_tabView->setShowGrid(true);
    m_tabView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_tabView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tabView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tabView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tabView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_tabView->verticalHeader()->setDefaultSectionSize(28);
    m_tabView->verticalHeader()->setVisible(false);
    m_tabView->horizontalHeader()->setFixedHeight(24);
    m_tabView->horizontalHeader()->setHighlightSections(false);
    m_tabView->horizontalHeader()->setStretchLastSection(true);
    m_tabView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    RetranslateUI(pView);
}

void ViewBase::ui_type::RetranslateUI(QWidget* pView)
{

}