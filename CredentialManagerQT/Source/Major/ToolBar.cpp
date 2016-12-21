#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>

#include "credential_qt_utils.h"
#include "credential_qt_delegate.h"

#include "Major/ToolBar.h"

QT_BEGIN_NAMESPACE

ToolBar::ToolBar(DelegateMainView* pDelegate, QWidget * parent)
    : QWidget(parent)
    , _delegate(pDelegate)
{
    _ui.SetupUI(this);

    QObject::connect(_ui.m_btnNew, &QPushButton::clicked, this, &ToolBar::OnClickedNew);
    QObject::connect(_ui.m_btnOpen, &QPushButton::clicked, this, &ToolBar::OnClickedOpen);
    QObject::connect(_ui.m_btnAbout, &QPushButton::clicked, this, &ToolBar::OnClickedAbout);
}

void ToolBar::UpdatePath(const QString & strPath)
{
    _ui.m_editPath->setText(strPath);
}

void ToolBar::OnClickedNew()
{
    if (_delegate)
        _delegate->OnClickedNew();
}

void ToolBar::OnClickedOpen()
{
    if (_delegate)
        _delegate->OnClickedOpen();
}

void ToolBar::OnClickedAbout()
{
    if (_delegate)
        _delegate->OnClickedAbout();
}

void ToolBar::ui_type::SetupUI(QWidget * pView)
{
    pView->setObjectName("ToolBar");
    ui_utils::SetBackgroundColor(pView, Qt::white);

    m_editPath = ui_utils::MakeShowLine(pView, ui_utils::g_clrLabel);

    m_btnOpen = ui_utils::MakeButton(pView);
    m_btnNew = ui_utils::MakeButton(pView);
    m_btnAbout = ui_utils::MakeButton(pView);
    m_btnOpen->setDefault(true);

    QHBoxLayout* phLayout = new QHBoxLayout;
    phLayout->setMargin(0);
    phLayout->setSpacing(8);
    phLayout->addWidget(m_editPath, 1);
    phLayout->addWidget(m_btnOpen);
    phLayout->addWidget(m_btnNew);

    QHBoxLayout* phMainLayout = new QHBoxLayout;
    phMainLayout->setMargin(4);
    phMainLayout->setSpacing(20);
    phMainLayout->addLayout(phLayout, 1);
    phMainLayout->addWidget(m_btnAbout);

    pView->setLayout(phMainLayout);
    pView->setFixedHeight(pView->sizeHint().height());

    RetranslateUI(pView);
}

void ToolBar::ui_type::RetranslateUI(QWidget * pView)
{
    m_btnOpen->setText("Open");
    m_btnNew->setText("New");
    m_btnAbout->setText("About");
}

QT_END_NAMESPACE
