#pragma once

#include <QMenuBar>

QT_BEGIN_NAMESPACE

class MenuBar : public QMenuBar
{
    struct ui_type
    {
        QMenu* m_menuFile;
        QMenu* m_menuHelp;

        QAction* m_actNew;
        QAction* m_actOpen;
        QAction* m_actAbout;

        void SetupUI(MenuBar* pView);
        void RetranslateUI(MenuBar* pView);
    };

public:

    MenuBar(QWidget* parent = nullptr);

    const ui_type& UI() const { return (_ui); }

private:

    void OnClickedNew();
    void OnClickedOpen();
    void OnClickedAbout();

    ui_type _ui;

};

QT_END_NAMESPACE
