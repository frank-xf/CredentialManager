#ifndef _bnb_Stack_View_H_
#define _bnb_Stack_View_H_

QT_BEGIN_NAMESPACE

class StackView : public QStackedWidget
{
public:

    StackView(QWidget * parent = nullptr);

    void InitCredential();
    void ClearCredential();

    bool UpdateCredential(unsigned int credential_id);
    bool UpdatePlatform(unsigned int credential_id, unsigned int platform_id);
    bool UpdateAccount(unsigned int platform_id, unsigned int account_id);
    bool UpdateProperty(unsigned int account_id, unsigned int property_id);

    bool RemovePlatform(unsigned int credential_id, const std::vector<unsigned int>& ids);
    bool RemoveAccount(unsigned int platform_id, const std::vector<unsigned int>& ids);
    bool RemoveProperty(unsigned int account_id, const std::vector<unsigned int>& ids);
    unsigned int RemoveView(const std::vector<unsigned int>& ids);

    bool AddPlatform(unsigned int credential_id, const bnb::platform_tree::data_type& platform);
    bool AddAccount(unsigned int platform_id, const bnb::account_tree::data_type& account);
    bool AddProperty(unsigned int account_id, const bnb::property_tree::data_type& property);

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
