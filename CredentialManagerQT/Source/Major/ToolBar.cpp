#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>

#include "credential_qt_utils.h"

#include "Major/ToolBar.h"

ToolBar::ToolBar(QWidget * parent) : QWidget(parent)
{
    ui_utils::SetBackgroundColor(this, Qt::white);

    _ui.SetupUI(this);

}

void ToolBar::UpdatePath(const QString & strPath)
{
    _ui.m_editPath->setText(strPath);
}

void ToolBar::ui_type::SetupUI(QWidget * pView)
{
    pView->setObjectName("ToolBar");

    m_editPath = new QLabel(pView);
    m_btnOpen = ui_utils::MakeButton(pView);
    m_btnNew = ui_utils::MakeButton(pView);
    m_btnAbout = ui_utils::MakeButton(pView);
    m_btnOpen->setDefault(true);

    QHBoxLayout* phLayout1 = new QHBoxLayout;
    phLayout1->setMargin(0);
    phLayout1->setSpacing(8);
    phLayout1->addWidget(m_editPath, 1);
    phLayout1->addWidget(m_btnOpen);
    phLayout1->addWidget(m_btnNew);

    QHBoxLayout* phMainLayout = new QHBoxLayout;
    phMainLayout->setMargin(4);
    phMainLayout->setSpacing(16);
    phMainLayout->addLayout(phLayout1, 1);
    phMainLayout->addWidget(m_btnAbout);

    pView->setLayout(phMainLayout);

    RetranslateUI(pView);
}

void ToolBar::ui_type::RetranslateUI(QWidget * pView)
{
    m_btnOpen->setText("Open");
    m_btnNew->setText("New");
    m_btnAbout->setText("About");
}
