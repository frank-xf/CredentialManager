#ifndef _bnb_Credential_Main_View_H_
#define _bnb_Credential_Main_View_H_

#include "bnb_global.h"
#include "Credential/Credential.h"

#include "credential_qt_delegate.h"

QT_BEGIN_NAMESPACE

class QPushButton;
class QScrollArea;
class QLabel;
class ToolBar;
class ContentView;
class QTreeWidget;

class MainView
    : public QWidget
    , public delegate_ns::credential_delegate
    , public delegate_ns::validate_delegate
{
    struct ui_type
    {
        QAction* m_actAddAccount;
        QAction* m_actAddPlatform;
        QAction* m_actAddProperty;
        QAction* m_actDelAccount;
        QAction* m_actDelPlatform;
        QAction* m_actDelProperty;
        QAction* m_actEditAccount;
        QAction* m_actEditPlatform;
        QAction* m_actEditProperty;
        QAction* m_actAlterPassword;
        QAction* m_actAlterUserName;
        
        ToolBar* m_viewNavigation;
        ContentView* m_viewContent;
        QTreeWidget* m_treeView;

        void SetupUI(MainView* pView);
        void RetranslateUI(MainView* pView);
    };

public:

    MainView(QWidget *parent = nullptr);

private:

    void ResetCredential();
    bool SaveCredential() const;

    void UpdateTitle();
    void UpdateCredentail();

    void OnClickedNew();
    void OnClickedOpen();
    void OnClickedAbout();
    void OnTreeContextMenu(const QPoint&);

    void OnClickedMotifyName();
    void OnClickedMotifyWord();

    bool OnAddPlatform() override;
    bool OnAddAccount(bnb::platform_type* pp) override;
    bool OnRemovePlatform(bnb::platform_type* pp) override;
    bool OnRemoveAccount(bnb::platform_type* pp, bnb::account_type* pa) override;
    bool OnEditPlatform(bnb::platform_type* pp) override;
    bool OnEditAccount(bnb::platform_type* pp, bnb::account_type* pa) override;
    bool OnViewCredential(bnb::platform_type* pp, bnb::account_type* pa) override;

    bool SetPlatform(bnb::platform_type& p1, const bnb::platform_type& p2) override;
    bool SetAccount(const bnb::platform_type& pp, bnb::account_type& a1, const bnb::account_type& a2) override;
    bool ValidateUserName(const bnb::string_type& username) override;
    bool ValidatePassword(const bnb::string_type& password) override;

private:

    ui_type _ui;

    QString m_strFile;
    bnb::Credential m_Credential;

};

QT_END_NAMESPACE

#endif // _bnb_Credential_Main_View_H_
