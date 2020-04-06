#ifndef _bnb_About_Dialog_H_
#define _bnb_About_Dialog_H_

QT_BEGIN_NAMESPACE

class QTextEdit;
class QPushButton;

class AboutDialog : public QDialog
{
    struct ui_type
    {
        QTextEdit* m_editText;
        QPushButton* m_btnClose;

        void SetupUI(AboutDialog* pView);
        void RetranslateUI(AboutDialog* pView);

    };

public:

	AboutDialog(QWidget * parent = nullptr);

private:

    ui_type _ui;

};

QT_END_NAMESPACE

#endif  // _bnb_About_Dialog_H_
