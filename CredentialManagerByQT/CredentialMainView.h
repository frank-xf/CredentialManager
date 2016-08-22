#ifndef _bnb_Credential_Main_View_H_
#define _bnb_Credential_Main_View_H_

#include "../CredentialManager/bnb_global.h"
#include "../CredentialManager/Credential/Credential.h"

QT_BEGIN_NAMESPACE

class QTableWidget;
class QPushButton;
class QLabel;
class QProgressBar;
class QScrollArea;
class CredentialView;

class CredentialMainView : public QWidget
{
    struct ui_type
    {
        QPushButton* m_btnNew;
        QPushButton* m_btnOpen;
        QPushButton* m_btnMotifyName;
        QPushButton* m_btnMotifyWord;
        QLabel* m_labFile;
        QLabel* m_labTime;
        QProgressBar* m_barTime;

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

    void UpdateCredentail(CredentialView* view);

private:

    bnb::Credential m_Credential;
    ui_type _ui;

};

QT_END_NAMESPACE

#endif // _bnb_Credential_Main_View_H_
