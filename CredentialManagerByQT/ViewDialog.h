#ifndef _bnb_View_Dialog_H_
#define _bnb_View_Dialog_H_

QT_BEGIN_NAMESPACE

class ViewDialog : public QDialog
{
    struct ui_type
    {
    private:

        QLabel* _labPlatform;
        QLabel* _labAccount;

    public:

        QComboBox* m_cboxPlatform;
        QComboBox* m_cboxAccount;

        QPushButton* m_btnUp;
        QPushButton* m_btnAdd;
        QPushButton* m_btnDown;
        QPushButton* m_btnClose;
        QPushButton* m_btnRemove;

        QTableWidget* m_tabProperty;

        void SetupUI(ViewDialog* pView);
        void RetranslateUI(ViewDialog* pView);
    };

public:

	ViewDialog(bnb::Credential& src, QWidget * parent = nullptr);
	
private:

    void InitView();

    void OnChangedPlatform(const QString& strText);
    void OnChangedAccount(const QString& strText);
    void OnDoubleClickedProperty(int row, int cln);
    void OnChangedCurrentProperty(int curRow, int curCln, int preRow, int preCln);
    void OnClickedUp();
    void OnClickedDown();
    void OnClickedAdd();
    void OnClickedRemove();

private:
	
    bnb::Credential& m_Credential;

    ui_type _ui;
};

QT_END_NAMESPACE

#endif  // _bnb_View_Dialog_H_
