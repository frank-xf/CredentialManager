#pragma once

QT_BEGIN_NAMESPACE

class QLabel;
class QLineEdit;
class QPushButton;

class LoginDialog : public QDialog
{
    struct ui_type
    {
    private:

        QLabel* _labUsername;
        QLabel* _labPassword;

    public:

        QLineEdit* m_editPassword;
        QLineEdit* m_editUsername;
        QPushButton* m_btnOK;
        QPushButton* m_btnCancel;

        void SetupUI(LoginDialog* pDlg);
        void RetranslateUI(LoginDialog* pDlg);
    };

public:

	LoginDialog(QWidget * parent = nullptr);

    QString GetPassword() const;
    QString GetUsername() const;

private:
	
    ui_type _ui;

};

QT_END_NAMESPACE
