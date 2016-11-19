#ifndef _bnb_Hint_Dialog_H_
#define _bnb_Hint_Dialog_H_

enum class hint_type : unsigned char { ht_error, ht_warning, ht_info, ht_max };

QT_BEGIN_NAMESPACE

class QLabel;
class QPushButton;

class HintDialog : public QDialog
{
    struct ui_type
    {
        QLabel* m_labText;
        QPushButton* m_btnClose;

        void SetupUI(HintDialog* pDlg);
        void RetranslateUI(HintDialog* pDlg);
    };

public:

    HintDialog(hint_type hType, const QString& strText, QWidget * parent);

private:
	
    ui_type _ui;

};

class ConfirmDialog : public QDialog
{
    struct ui_type
    {
        QLabel* m_labText;
        QPushButton* m_btnOK;
        QPushButton* m_btnCancel;

        void SetupUI(ConfirmDialog* pDlg);
        void RetranslateUI(ConfirmDialog* pDlg);
    };

public:

    ConfirmDialog(const QString& strText, QWidget * parent);

private:

    ui_type _ui;

};

QT_END_NAMESPACE

#endif  // _bnb_Hint_Dialog_H_