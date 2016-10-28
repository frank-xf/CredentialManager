#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>

#include "credential_qt_utils.h"

#include "View/ViewBase.h"
#include "View/CredentialView.h"

CredentialView::CredentialView(QWidget * parent) : ViewBase(0, parent)
{
    _ui.SetupUI(this);
}

void CredentialView::ui_type::SetupUI(QWidget* pView)
{
    ViewBase::ui_type::SetupUI(pView);

    m_tabView->setColumnCount(3);

    _labUpdateTime = ui_utils::MakeLabel(pView, 60, 24, Qt::AlignVCenter | Qt::AlignRight, Qt::black, "", 16);
    _labUserName = ui_utils::MakeLabel(pView, 60, 24, Qt::AlignVCenter | Qt::AlignRight, Qt::black, "", 16);
    _labDisplay = ui_utils::MakeLabel(pView, 60, 24, Qt::AlignVCenter | Qt::AlignRight, Qt::black, "", 16);



    RetranslateUI(pView);
}

void CredentialView::ui_type::RetranslateUI(QWidget* pView)
{
    m_tabView->setHorizontalHeaderLabels({ "Platform", "Url", "Display" });

    _labUpdateTime->setText("Update Time: ");
    _labUserName->setText("User Name: ");
    _labDisplay->setText("Display: ");
}