#ifndef _bnb_Credential_View_H_
#define _bnb_Credential_View_H_

QT_BEGIN_NAMESPACE

class CredentialView : public QWidget, public delegate_ns::credential_delegate
{
    using delegate_type = delegate_ns::credential_delegate;

    struct ui_type
    {
        QAction* m_actAddPlatform;

        void SetupUI(CredentialView* pView);
        void RetranslateUI(CredentialView* pView);
    };

public:

    CredentialView(bnb::Credential& src, delegate_type* ptrDelegate, QWidget * parent);

    void contextMenuEvent(QContextMenuEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:

    void LayoutView(bnb::platform_list& listPlatform);
    void OnClickedAddPlatform();

    bool OnAddPlatform() override;
    bool OnAddAccount() override;
    bool OnRemovePlatform(bnb::platform_type* pp) override;
    bool OnRemoveAccount(bnb::platform_type* pp, bnb::account_type* pa) override;
    bool OnEditPlatform(bnb::platform_type* pp) override;
    bool OnEditAccount(bnb::platform_type* pp, bnb::account_type* pa) override;
    bool OnViewCredential(bnb::platform_type* pp, bnb::account_type* pa) override;

private:

    ui_type _ui;

    delegate_type* m_ptrDelegate;
};

QT_END_NAMESPACE

#endif // _bnb_Credential_View_H_
