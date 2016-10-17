#ifndef _bnb_Edit_Dialog_H_
#define _bnb_Edit_Dialog_H_

QT_BEGIN_NAMESPACE

class QLabel;
class QLineEdit;
class QPushButton;

//------------------------------------------------------------------------------

class EditCredentialDialog : public QDialog
{
    struct ui_type
    {
    private:
        
        QLabel* _labUserName;

    public:

        QLabel* m_labHint;
        QLineEdit* m_editUserName;

        QPushButton* m_btnOK;
        QPushButton* m_btnCancel;

        void SetupUI(EditCredentialDialog* pView);
        void RetranslateUI(EditCredentialDialog* pView);
    };

public:

    EditCredentialDialog(QWidget* parent = nullptr);

private:

    void OnChangedText(const QString&);
    void OnClickedOK();
    
private:

    ui_type _ui;

};  // class EditUserNameDialog

//------------------------------------------------------------------------------

class EditPasswordDialog : public QDialog
{
    struct ui_type
    {
    private:

        QLabel* _labOldPassword;
        QLabel* _labNewPassword;
        QLabel* _labValidate;

    public:

        QLabel* m_labHint;
        QLineEdit* m_editOldPassword;
        QLineEdit* m_editNewPassword;
        QLineEdit* m_editValidate;

        QPushButton* m_btnOK;
        QPushButton* m_btnCancel;

        void SetupUI(EditPasswordDialog* pView);
        void RetranslateUI(EditPasswordDialog* pView);
    };

public:

    EditPasswordDialog(QWidget* parent = nullptr);

private:

    void OnChangedText(const QString&);
    void OnClickedOK();

private:

    ui_type _ui;

};  // class EditPasswordDialog

//------------------------------------------------------------------------------

class EditPlatformDialog : public QDialog
{
    struct ui_type
    {
    private:

        QLabel* _labPlatform;
        QLabel* _labUrl;
        QLabel* _labDisplay;

    public:

        QLabel* m_labHint;

        QLineEdit* m_editPlatform;
        QLineEdit* m_editUrl;
        QLineEdit* m_editDisplay;

        QPushButton* m_btnOK;
        QPushButton* m_btnCancel;

        void SetupUI(EditPlatformDialog* pView);
        void RetranslateUI(EditPlatformDialog* pView);
    };

public:

	EditPlatformDialog(bnb::platform_type* platform, QWidget* parent);

private:

    void OnChangedText(const QString&);
    void OnClickedOK();

private:
	
    ui_type _ui;

    bnb::platform_type* m_Platform;

};  // class EditPlatformDialog

//------------------------------------------------------------------------------

class EditAccountDialog : public QDialog
{
    struct ui_type
    {
    private:

        QLabel* _labPlatform;
        QLabel* _labAccount;
        QLabel* _labDisplay;

    public:

        QLabel* m_labHint;

        QLineEdit* m_editPlatform;
        QLineEdit* m_editAccount;
        QLineEdit* m_editDisplay;

        QPushButton* m_btnOK;
        QPushButton* m_btnCancel;

        void SetupUI(EditAccountDialog* pView);
        void RetranslateUI(EditAccountDialog* pView);
    };

public:

    EditAccountDialog(bnb::platform_type* pp, bnb::account_type* pa, QWidget * parent);
	const bnb::account_type* GetAccount() const;


    void OnChangedText(const QString&);
    void OnClickedOK();

private:

    ui_type _ui;

    bnb::account_type* m_Account;
    bnb::platform_type* m_Platform;

};  // class EditAccountDialog

//------------------------------------------------------------------------------

class EditPropertyDialog : public QDialog
{

};  // class EditPropertyDialog

QT_END_NAMESPACE

#endif  // _bnb_Edit_Dialog_H_
