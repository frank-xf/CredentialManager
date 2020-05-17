#pragma once

QT_BEGIN_NAMESPACE

class StackView : public QStackedWidget
{
    struct ui_type
    {
        // ToolBar* m_viewToolBar;
        // StackView* m_viewStack;
        // TreeView* m_treeView;

        QLabel* m_labHint;

        void SetupUI(QWidget* pView);
        void RetranslateUI(QWidget* pView);
    };

public:

    StackView(QWidget * parent = nullptr);

    void ClearCredential();
    /*
    void AddCredential(const bnb::Credential& credential);
    bool AddPlatform(const bnb::platform_node& platform);
    bool AddAccount(const bnb::account_node& account);
    bool AddPair(const bnb::pair_node& pair);
    */
    /*
    bool UpdateCredential(id_type credentialId);
    bool UpdatePlatform(id_type credentialId, id_type platformId);
    bool UpdateAccount(id_type credentialId, id_type platformId, id_type accountId);
    bool UpdatePair(id_type credentialId, id_type accountId, id_type pairId);

    bool UpdateTable(id_type credentialId);
    bool UpdateTable(id_type credentialId, id_type platformId);
    bool UpdateTable(id_type credentialId, id_type platformId, id_type accountId);

	bool RemoveCredential(id_type credentialId, const std::vector<id_type>& ids);
	bool RemovePlatform(id_type credentialId, id_type platformId, const std::vector<id_type>& ids);
    bool RemoveAccount(id_type credentialId, id_type platformId, id_type accountId, const std::vector<id_type>& ids);
    bool RemovePair(id_type credentialId, id_type platformId, id_type accountId, id_type pairId);
    unsigned int RemoveView(const std::vector<id_type>& ids);
    */
    bool SwitchToView(xf::credential::credential_type eType);
    void SwitchToHint();

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
