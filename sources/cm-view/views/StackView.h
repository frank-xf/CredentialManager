#pragma once

QT_BEGIN_NAMESPACE

class StackView : public QStackedWidget
{
    struct ui_type
    {
        QLabel* m_labHint;

        CredentialView* m_viewCredential;
        PlatformView* m_viewPlatform;
        AccountView* m_viewAccount;

        void SetupUI(QWidget* pView);
        void RetranslateUI(QWidget* pView);
    };

public:

    StackView(QWidget * parent = nullptr);

    void ClearCredential();
    void InitCredential(const xf::credential::credential_t& credential);

    void SwitchToView(const xf::credential::credential_t& credential);
    void SwitchToView(const xf::credential::platform_t& platform);
    void SwitchToView(const xf::credential::account_t& account);
    void SwitchToView();

private:

    template<typename _Ty>
    bool UpdateView(xf::credential::credential_type eType)
    {
        /*
        bool bView1 = false;
        bool bView2 = false;
        for (int i = 0; i < count(); ++i)
        {
            if (!bView1)
            {
                _Ty* ptr = dynamic_cast<_Ty*>(widget(i));
                if (ptr && ptr->GetID() == viewId)
                {
                    ptr->UpdateTable();
                    bView1 = true;
                }
            }
            if (!bView2)
            {
                CredentialView* ptr = dynamic_cast<CredentialView*>(widget(i));
                if (ptr && ptr->GetID() == credentialId)
                {
                    ptr->UpdateInfo();
                    bView2 = true;
                }
            }

            if (bView1 && bView2) return true;
        }
        */
        return false;
    }

    ui_type _ui;
};

QT_END_NAMESPACE
