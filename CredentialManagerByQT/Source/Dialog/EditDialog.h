#ifndef _bnb_Edit_Dialog_H_
#define _bnb_Edit_Dialog_H_

QT_BEGIN_NAMESPACE

class QLabel;
class QLineEdit;
class QPushButton;

//------------------------------------------------------------------------------

class EditUserNameDialog : public QDialog
{
    using delegate_type = delegate_ns::validate_delegate;

    struct ui_type
    {
    private:
        
        QLabel* _labUserName;

    public:

        QLabel* m_labHint;
        QLineEdit* m_editUserName;

        QPushButton* m_btnOK;
        QPushButton* m_btnCancel;

        void SetupUI(EditUserNameDialog* pView);
        void RetranslateUI(EditUserNameDialog* pView);
    };

public:

    EditUserNameDialog(const QString& strUserName, delegate_type* ptrDelegate, QWidget* parent);

    QString GetUserName() const;

private:

    void OnChangedText(const QString&);
    void OnClickedOK();
    
private:

    ui_type _ui;

    delegate_type* m_ptrDelegate;

};  // class EditUserNameDialog

//------------------------------------------------------------------------------

class EditPasswordDialog : public QDialog
{
    using delegate_type = delegate_ns::validate_delegate;

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

    EditPasswordDialog(delegate_type* ptrDelegate, QWidget* parent);

    QString GetPassword() const;

private:

    void OnChangedText(const QString&);
    void OnClickedOK();

private:

    ui_type _ui;

    delegate_type* m_ptrDelegate;

};  // class EditPasswordDialog

//------------------------------------------------------------------------------

class EditPlatformDialog : public QDialog
{
    using delegate_type = delegate_ns::validate_delegate;

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

	EditPlatformDialog(bnb::platform_type& platform, delegate_type* ptrDelegate, QWidget* parent);

private:

    void OnChangedText(const QString&);
    void OnClickedOK();

private:
	
    ui_type _ui;

    delegate_type* m_ptrDelegate;
    bnb::platform_type& m_Platform;

};  // class EditPlatformDialog

//------------------------------------------------------------------------------

class EditAccountDialog : public QDialog
{
    using delegate_type = delegate_ns::validate_delegate;

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

    EditAccountDialog(bnb::platform_type& platform, bnb::account_type& account, delegate_type* ptrDelegate, QWidget * parent);

    void OnChangedText(const QString&);
    void OnClickedOK();

private:

    ui_type _ui;

    bnb::account_type& m_Account;
    bnb::platform_type& m_Platform;
    delegate_type* m_ptrDelegate;

};  // class EditAccountDialog

//------------------------------------------------------------------------------

class EditPropertyDialog : public QDialog
{

};  // class EditPropertyDialog

QT_END_NAMESPACE

#endif  // _bnb_Edit_Dialog_H_
