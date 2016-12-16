#ifndef _bnb_Stack_View_H_
#define _bnb_Stack_View_H_

QT_BEGIN_NAMESPACE

class StackView : public QStackedWidget
{
public:

    StackView(QWidget * parent = nullptr);

    void InitCredential(const bnb::Credential& credential);
    void ClearCredential();

    bool UpdateCredential(unsigned int credential_id);
    bool UpdatePlatform(unsigned int credential_id, unsigned int platform_id);
    bool UpdateAccount(unsigned int platform_id, unsigned int account_id);
    bool UpdateProperty(unsigned int account_id, unsigned int property_id);

    bool RemovePlatform(unsigned int credential_id, const std::vector<unsigned int>& ids);
    bool RemoveAccount(unsigned int platform_id, const std::vector<unsigned int>& ids);
    bool RemoveProperty(unsigned int account_id, const std::vector<unsigned int>& ids);
    unsigned int RemoveView(const std::vector<unsigned int>& ids);

    bool AddCredential(const bnb::Credential& credential);
    bool AddPlatform(const bnb::platform_node& platform);
    bool AddAccount(const bnb::account_node& account);
    bool AddProperty(const bnb::property_node& property);

    bool SwitchToCredential(unsigned int id);
    bool SwitchToPlatform(unsigned int id);
    bool SwitchToAccount(unsigned int id);
    bool SwitchToView(unsigned int id);
    bool SwitchToHint(const QString& strText);

private:

    QLabel* m_labHint;

};

QT_END_NAMESPACE

#endif  // _bnb_Stack_View_H_
