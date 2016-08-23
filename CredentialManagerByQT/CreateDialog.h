#ifndef _bnb_Create_Dialog_H_
#define _bnb_Create_Dialog_H_

QT_BEGIN_NAMESPACE

class CreateDialog : public QDialog
{
    struct ui_type
    {
        QPushButton* m_btnOK;
        QPushButton* m_btnCancel;


    };



public:
	CreateDialog(QWidget * parent = nullptr);
	~CreateDialog();

private:
	
};

QT_END_NAMESPACE

#endif  // _bnb_Create_Dialog_H_
