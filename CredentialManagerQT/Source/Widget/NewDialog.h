#ifndef _bnb_Create_Dialog_H_
#define _bnb_Create_Dialog_H_

QT_BEGIN_NAMESPACE

class QLabel;
class QPushButton;
class QLineEdit;

class NewDialog : public QDialog
{
    struct ui_type
    {
    private:

        QLabel* _labUserName;
        QLabel* _labPassword;
        QLabel* _labValidate;
        QLabel* _labComment;

    public:

        QLabel* m_labHint;

        QPushButton* m_btnOK;
        QPushButton* m_btnCancel;
        QPushButton* m_btnBrowse;

        QLineEdit* m_editUserName;
        QLineEdit* m_editPassword;
        QLineEdit* m_editValidate;
        QLineEdit* m_editFilePath;
        QLineEdit* m_editComment;

        void SetupUI(NewDialog* pView);
        void RetranslateUI(NewDialog* pView);
    };

public:

	NewDialog(QWidget * parent = nullptr);

    QString GetFileName() const;
    QString GetUserName() const;
    QString GetPassword() const;
    QString GetComment() const;

private:

    void OnClickedOK();
    void OnClickedBrowse();

private:
	
    ui_type _ui;
};

QT_END_NAMESPACE

#endif  // _bnb_Create_Dialog_H_
