#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>

#include "cm_view_utils.h"

#include "views/ToolBar.h"

QT_BEGIN_NAMESPACE

ToolBar::ToolBar(QWidget * parent)
    : QWidget(parent)
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
    /*
    if (_delegate)
        _delegate->OnClickedNew();
        */
}

void ToolBar::OnClickedOpen()
{
    /*
    if (_delegate)
        _delegate->OnClickedOpen();
        */
}

void ToolBar::OnClickedAbout()
{
    /*
    if (_delegate)
        _delegate->OnClickedAbout();
        */
}

void ToolBar::ui_type::SetupUI(QWidget * pView)
{
    pView->setObjectName("ToolBar");
    SetBackgroundColor(pView, Qt::white);

    m_editPath = MakeShowLine(pView, g_clrLabel);

    m_btnOpen = MakeButton(pView);
    m_btnNew = MakeButton(pView);
    m_btnAbout = MakeButton(pView);
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
