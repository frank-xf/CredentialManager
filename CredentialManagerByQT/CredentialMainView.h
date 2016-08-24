#ifndef _bnb_Credential_Main_View_H_
#define _bnb_Credential_Main_View_H_

#include "../CredentialManager/bnb_global.h"
#include "../CredentialManager/Credential/Credential.h"

#include "CredentialView.h"

QT_BEGIN_NAMESPACE

class QTableWidget;
class QPushButton;
class QLabel;
class QProgressBar;
class QScrollArea;

class CredentialMainView : public QWidget, public CredentialView::delegate_type
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
    ~CredentialMainView();

public:

    void OnClickedNew();
    void OnClickedOpen();
    void OnClickedMotifyName();
    void OnClickedMotifyWord();

private:

    void UpdateCredentail();
    void UpdateSize();

    bool OnAddPlatform() override;
    bool OnAddAccount() override;
    bool OnRemovePlatform(bnb::platform_type* pp) override;
    bool OnRemoveAccount(bnb::platform_type* pp, bnb::account_type* pa) override;
    bool OnEditPlatform(bnb::platform_type* pp) override;
    bool OnEditAccount(bnb::platform_type* pp, bnb::account_type* pa) override;
    bool OnViewCredential(bnb::platform_type* pp, bnb::account_type* pa) override;

private:

    ui_type _ui;

    QString m_strFile;
    bnb::Credential m_Credential;
};

QT_END_NAMESPACE

#endif // _bnb_Credential_Main_View_H_
