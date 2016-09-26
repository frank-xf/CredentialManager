#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>

#include "Major/ToolBar.h"

ToolBar::ToolBar(QWidget * parent) : QWidget(parent)
{
    _ui.SetupUI(this);

}

void ToolBar::ui_type::SetupUI(QWidget * pView)
{
    pView->setObjectName("ToolBar");

    m_editPath = new QLabel(pView);
    m_btnOpen = new QPushButton(pView);
    m_btnNew = new QPushButton(pView);
    m_btnAbout = new QPushButton(pView);

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
