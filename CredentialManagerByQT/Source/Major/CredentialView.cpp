﻿#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMenu>
#include <QtWidgets/QLabel>
#include <QtGui/QContextMenuEvent>

#include "bnb_global.h"
#include "Credential/Credential.h"

#include "credential_qt_delegate.h"
#include "credential_qt_utils.h"

#include "Major/CredentialItem.h"
#include "Major/CredentialView.h"

QT_BEGIN_NAMESPACE

CredentialView::CredentialView(bnb::Credential& src, delegate_type* ptrDelegate, QWidget * parent)
    : QWidget(parent)
    , m_ptrDelegate(ptrDelegate)
{
    ui_utils::SetBackgroundColor(this, QColor(255, 128, 128));

    _ui.SetupUI(this);

    LayoutView(src.Tree());

    QObject::connect(_ui.m_actAddPlatform, &QAction::triggered, this, &CredentialView::OnClickedAddPlatform);

    qDebug("w: %d, h: %d", width(), height());
}

void CredentialView::contextMenuEvent(QContextMenuEvent * event)
{
    QMenu menu(this);

    menu.addAction(_ui.m_actAddPlatform);

    menu.exec(event->globalPos());
}

void CredentialView::mouseDoubleClickEvent(QMouseEvent * event)
{
}

void CredentialView::LayoutView(bnb::platform_list & listPlatform)
{
    QVBoxLayout* pLayout = new QVBoxLayout;
    pLayout->setSpacing(2);
    pLayout->setMargin(1);

    if (listPlatform.IsEmpty())
    {
        QLabel* labHint = new QLabel(this);
        labHint->setAlignment(Qt::AlignCenter);
        labHint->setMinimumSize(ui_utils::item_platform_w, ui_utils::item_platform_h);
        labHint->setText("You haven\'t added any Credential !");

        pLayout->addWidget(labHint);
    }
    else
    {
        for (auto ptr_platform = listPlatform.Head(); ptr_platform; ptr_platform = ptr_platform->m_Next)
        {
            CredentialItem* pItem = new CredentialItem(ptr_platform->m_Pair.m_Key, ptr_platform->m_Pair.m_Value, this, this);
            pLayout->addWidget(pItem);
        }
    }

    setLayout(pLayout);
    setFixedSize(sizeHint());
}

void CredentialView::OnClickedAddPlatform()
{
    m_ptrDelegate->OnAddPlatform();
}

void CredentialView::ui_type::SetupUI(CredentialView * pView)
{
    m_actAddPlatform = new QAction(pView);

    RetranslateUI(pView);
}

void CredentialView::ui_type::RetranslateUI(CredentialView * pView)
{
    m_actAddPlatform->setText("Add Platform");
}

bool CredentialView::OnAddPlatform()
{
    return m_ptrDelegate->OnAddPlatform();
}

bool CredentialView::OnAddAccount(bnb::platform_type* pp)
{
    return m_ptrDelegate->OnAddAccount(pp);
}

bool CredentialView::OnRemovePlatform(bnb::platform_type * pp)
{
    return m_ptrDelegate->OnRemovePlatform(pp);
}

bool CredentialView::OnRemoveAccount(bnb::platform_type * pp, bnb::account_type * pa)
{
    return m_ptrDelegate->OnRemoveAccount(pp, pa);
}

bool CredentialView::OnEditPlatform(bnb::platform_type * pp)
{
    return m_ptrDelegate->OnEditPlatform(pp);
}

bool CredentialView::OnEditAccount(bnb::platform_type * pp, bnb::account_type * pa)
{
    return m_ptrDelegate->OnEditAccount(pp, pa);
}

bool CredentialView::OnViewCredential(bnb::platform_type * pp, bnb::account_type * pa)
{
    return m_ptrDelegate->OnViewCredential(pp, pa);
}

QT_END_NAMESPACE
