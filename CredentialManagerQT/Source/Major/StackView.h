#ifndef _bnb_Stack_View_H_
#define _bnb_Stack_View_H_

QT_BEGIN_NAMESPACE

class StackView : public QStackedWidget
{
    using id_type = bnb::Credential::id_type;

public:

    StackView(DelegateType* pDelegate = nullptr, QWidget * parent = nullptr);

    void ClearCredential();

    void AddCredential(const bnb::Credential& credential);
    bool AddPlatform(const bnb::platform_node& platform);
    bool AddAccount(const bnb::account_node& account);
    bool AddPair(const bnb::pair_node& pair);

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

    bool SwitchToView(bnb::credential_enum eType, id_type id);
    void SwitchToHint();

private:

    template<typename _Ty>
    bool UpdateView(id_type credentialId, id_type viewId)
    {
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

        return false;
    }

    QLabel* m_labHint;

    DelegateType* _delegate;
};

QT_END_NAMESPACE

#endif  // _bnb_Stack_View_H_
