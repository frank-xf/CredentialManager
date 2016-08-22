#ifndef _bnb_Hint_Dialog_H_
#define _bnb_Hint_Dialog_H_

class HintDialog : public QDialog
{
    struct ui_type
    {
        QLabel* m_labText;
        QPushButton* m_btnOK;

        void SetupUI(HintDialog* pDlg);
        void RetranslateUI(HintDialog* pDlg);
    };

public:
    HintDialog(const QString& strText = {}, const QString& strTitle = {}, QWidget * parent = nullptr);
	~HintDialog();

    void SetText(const QString& strText);
    QString GetText() const;

private:
	
    ui_type _ui;

};

#endif  // _bnb_Hint_Dialog_H_