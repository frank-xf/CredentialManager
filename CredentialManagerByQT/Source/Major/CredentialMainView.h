#ifndef _bnb_Credential_Main_View_H_
#define _bnb_Credential_Main_View_H_

#include "bnb_global.h"
#include "Credential/Credential.h"

#include "credential_qt_delegate.h"

QT_BEGIN_NAMESPACE

class QPushButton;
class QScrollArea;
class QLabel;

class CredentialMainView
    : public QWidget
    , public delegate_ns::credential_delegate
    , public delegate_ns::validate_delegate
{
    struct ui_type
    {
        QPushButton* m_btnNew;
        QPushButton* m_btnOpen;
        QPushButton* m_btnMotifyName;
        QPushButton* m_btnMotifyWord;

        QScrollArea* m_areaCredential;

        void SetupUI(CredentialMainView* pView);
        void RetranslateUI(CredentialMainView* pView);
    };

public:

    CredentialMainView(QWidget *parent = nullptr);

private:

    bool SaveCredential() const;

    void UpdateSize();
    void UpdateTitle();
    void UpdateCredentail();

    void OnClickedNew();
    void OnClickedOpen();
    void OnClickedMotifyName();
    void OnClickedMotifyWord();

    bool OnAddPlatform() override;
    bool OnAddAccount(bnb::platform_type* pp) override;
    bool OnRemovePlatform(bnb::platform_type* pp) override;
    bool OnRemoveAccount(bnb::platform_type* pp, bnb::account_type* pa) override;
    bool OnEditPlatform(bnb::platform_type* pp) override;
    bool OnEditAccount(bnb::platform_type* pp, bnb::account_type* pa) override;
    bool OnViewCredential(bnb::platform_type* pp, bnb::account_type* pa) override;

    bool ValidateUserName(const bnb::string_type& username) override;
    bool ValidatePassword(const bnb::string_type& password) override;
    bool SetPlatform(bnb::platform_type& p1, const bnb::platform_type& p2) override;
    bool SetAccount(const bnb::platform_type& pp, bnb::account_type& a1, const bnb::account_type& a2) override;

private:

    ui_type _ui;

    QString m_strFile;
    bnb::Credential m_Credential;
};

QT_END_NAMESPACE

#endif // _bnb_Credential_Main_View_H_