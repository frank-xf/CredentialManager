#ifndef _bnb_Credential_Item_H_
#define _bnb_Credential_Item_H_

QT_BEGIN_NAMESPACE

class QLabel;

//------------------------------------------------------------------------------

class AccountLabel : public QWidget
{
    struct ui_type
    {
        QAction* m_actView;
        QAction* m_actEdit;
        QAction* m_actRemove;

        QLabel* m_labAccount;

        void SetupUI(AccountLabel* pView);
        void RetranslateUI(AccountLabel* pView);
    };

public:

    struct delegate_type
    {
        ~delegate_type() { }

        virtual bool OnViewCredential(bnb::account_type* pAccount) = 0;
        virtual bool OnEditAccount(bnb::account_type* pAccount) = 0;
        virtual bool OnRemoveAccount(bnb::account_type* pAccount) = 0;
    };

    AccountLabel(bnb::account_type* pAccount, delegate_type* ptrDelegate, QWidget* parent);

    void Update();

private:

    void contextMenuEvent(QContextMenuEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void OnClickedView();
    void OnClickedEdit();
    void OnClickedRemove();

private:

    ui_type _ui;

    bnb::account_type* m_ptrAccount;
    delegate_type* m_ptrDelegate;

};  // class AccountLabel

//------------------------------------------------------------------------------

class PlatformLabel : public QWidget
{
    struct ui_type
    {
        QAction* m_actView;
        QAction* m_actEdit;
        QAction* m_actRemove;
        QAction* m_actAddPlatform;
        QAction* m_actAddAccount;

        QLabel* m_labPlatform;

        void SetupUI(PlatformLabel* pView);
        void RetranslateUI(PlatformLabel* pView);
    };

public:

    struct delegate_type
    {
        ~delegate_type() { }

        virtual bool OnEditPlatform(bnb::platform_type* ptrPlatform) = 0;
        virtual bool OnRemovePlatform(bnb::platform_type* ptrPlatform) = 0;
        virtual bool OnViewCredential(bnb::platform_type* ptrPlatform) = 0;
        virtual bool OnAddPlatform() = 0;
        virtual bool OnAddAccount() = 0;
    };

    PlatformLabel(bnb::platform_type* ptrPlatform, delegate_type* ptrDelegate, QWidget* parent);

    void Update();
    bnb::platform_type* GetPlatform() { return m_ptrPlatform; }

private:

    void contextMenuEvent(QContextMenuEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void OnClickedView();
    void OnClickedEdit();
    void OnClickedRemove();
    void OnClickedAddPlatform();
    void OnClickedAddAccount();

private:

    ui_type _ui;

    delegate_type* m_ptrDelegate;
    bnb::platform_type* m_ptrPlatform;

};  // class PlatformLabel

//------------------------------------------------------------------------------

class CredentialItem
    : public QWidget
    , public AccountLabel::delegate_type
    , public PlatformLabel::delegate_type
{
public:

    struct delegate_type
    {
        ~delegate_type() { }

        virtual bool OnAddPlatform() = 0;
        virtual bool OnAddAccount() = 0;
        virtual bool OnRemovePlatform(bnb::platform_type* pp) = 0;
        virtual bool OnRemoveAccount(bnb::platform_type* pp, bnb::account_type* pa) = 0;
        virtual bool OnEditPlatform(bnb::platform_type* pp) = 0;
        virtual bool OnEditAccount(bnb::platform_type* pp, bnb::account_type* pa) = 0;
        virtual bool OnViewCredential(bnb::platform_type* pp, bnb::account_type* pa) = 0;
    };

	CredentialItem(bnb::platform_type& platform, bnb::account_list& account, delegate_type* ptrDelegate, QWidget* parent);

private:

    bool OnAddPlatform() override;
    bool OnAddAccount() override;
    bool OnEditPlatform(bnb::platform_type* ptrPlatform) override;
    bool OnEditAccount(bnb::account_type* pAccount) override;
    bool OnRemoveAccount(bnb::account_type* pAccount) override;
    bool OnRemovePlatform(bnb::platform_type* ptrPlatform) override;
    bool OnViewCredential(bnb::platform_type* ptrPlatform) override;
    bool OnViewCredential(bnb::account_type* pAccount) override;

    delegate_type* m_ptrDelegate;
    PlatformLabel* m_labPlatform;

};  // class CredentialItem

QT_END_NAMESPACE

#endif  // _bnb_Credential_Item_H_
