#ifndef _bnb_Password_Input_H_
#define _bnb_Password_Input_H_

QT_BEGIN_NAMESPACE

class QLabel;
class QLineEdit;
class QPushButton;

class PasswordInput : public QDialog
{
    struct ui_type
    {
    private:

        QLabel* _labPassword;

    public:

        QLineEdit* m_editPassword;
        QPushButton* m_btnOK;
        QPushButton* m_btnCancel;

        void SetupUI(PasswordInput* pDlg);
        void RetranslateUI(PasswordInput* pDlg);
    };

public:

	PasswordInput(QWidget * parent = nullptr);

    QString GetPassword() const;

private:
	
    ui_type _ui;

};

QT_END_NAMESPACE

#endif // _bnb_Password_Input_H_
