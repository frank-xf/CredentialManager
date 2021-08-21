#include <QtWidgets/QMenuBar>


#include "../cm_view_utils.h"

#include "MenuBar.h"


QT_BEGIN_NAMESPACE

MenuBar::MenuBar(QWidget* parent)
    : QMenuBar(parent)
{
    _ui.SetupUI(this);

    /*
    QObject::connect(_ui.m_btnNew, &QPushButton::clicked, this, &ToolBar::OnClickedNew);
    QObject::connect(_ui.m_btnOpen, &QPushButton::clicked, this, &ToolBar::OnClickedOpen);
    QObject::connect(_ui.m_btnAbout, &QPushButton::clicked, this, &ToolBar::OnClickedAbout);
    */
}



void MenuBar::ui_type::SetupUI(MenuBar* pView)
{
    pView->setObjectName("MenuBar");
    pView->setFixedHeight(40);
    SetBackgroundColor(pView, Qt::white);

    auto menu_style = "QMenu{ border: 1px solid gray; background-color: white; }"
        "QMenu::item{ padding: 2px 16px 2px 16px; }"
        "QMenu::item:selected{ background:#C0F0C0;}"
        "QMenu::separator{ height: 1px; background: gray; margin-left: 2px; margin-right: 2px;}";

    m_menuFile = new QMenu(pView);
    m_menuFile->setStyleSheet(menu_style);
    m_menuHelp = new QMenu(pView);
    m_menuHelp->setStyleSheet(menu_style);

    m_actNew = new QAction(m_menuFile);
    m_actOpen = new QAction(m_menuFile);
    m_actAbout = new QAction(m_menuHelp);

    m_menuFile->addAction(m_actNew);
    m_menuFile->addAction(m_actOpen);
    m_menuHelp->addAction(m_actAbout);

    pView->addMenu(m_menuFile);
    pView->addMenu(m_menuHelp);

    RetranslateUI(pView);
}

void MenuBar::ui_type::RetranslateUI(MenuBar* pView)
{
    m_menuFile->setTitle("File");
    m_menuHelp->setTitle("Help");

    m_actOpen->setText("Open");
    m_actNew->setText("New");
    m_actAbout->setText("About");
}

QT_END_NAMESPACE
