#ifndef _bnb_Stack_View_H_
#define _bnb_Stack_View_H_

QT_BEGIN_NAMESPACE

class StackView : public QStackedWidget
{
public:

    StackView(DelegateMainView* pDelegate = nullptr, QWidget * parent = nullptr);

    void ClearCredential();

    void AddCredential(const bnb::Credential& credential);
    bool AddPlatform(const bnb::platform_node& platform);
    bool AddAccount(const bnb::account_node& account);
    bool AddPair(const bnb::pair_node& pair);

    bool UpdateCredential(unsigned int credential_id);
    bool UpdatePlatform(unsigned int credential_id, unsigned int platform_id);
    bool UpdateAccount(unsigned int credential_id, unsigned int platform_id, unsigned int account_id);
    bool UpdatePair(unsigned int credential_id, unsigned int account_id, unsigned int pair_id);

    bool UpdateTable(unsigned int credential_id);
    bool UpdateTable(unsigned int credential_id, unsigned int platform_id);
    bool UpdateTable(unsigned int credential_id, unsigned int platform_id, unsigned int account_id);

    bool RemovePlatform(unsigned int credential_id, unsigned int platform_id, const std::vector<unsigned int>& ids);
    bool RemoveAccount(unsigned int credential_id, unsigned int platform_id, unsigned int account_id, const std::vector<unsigned int>& ids);
    bool RemovePair(unsigned int credential_id, unsigned int platform_id, unsigned int account_id, unsigned int pair_id);
    unsigned int RemoveView(const std::vector<unsigned int>& ids);

    bool SwitchToView(bnb::credential_enum eType, unsigned int id);
    bool SwitchToHint(const QString& strText);

private:

    template<typename _Ty>
    bool UpdateView(unsigned int credential_id, unsigned int view_id)
    {
        bool bView1 = false;
        bool bView2 = false;
        for (int i = 0; i < count(); ++i)
        {
            if (!bView1)
            {
                _Ty* ptr = dynamic_cast<_Ty*>(widget(i));
                if (ptr && ptr->GetID() == view_id)
                {
                    ptr->UpdateTable();
                    bView1 = true;
                }
            }
            if (!bView2)
            {
                CredentialView* ptr = dynamic_cast<CredentialView*>(widget(i));
                if (ptr && ptr->GetID() == credential_id)
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

    DelegateMainView* _delegate;
};

QT_END_NAMESPACE

#endif  // _bnb_Stack_View_H_
