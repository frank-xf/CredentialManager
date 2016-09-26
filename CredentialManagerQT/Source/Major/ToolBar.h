
#ifndef _bnb_Navigation_View_H_
#define _bnb_Navigation_View_H_

class ToolBar : public QWidget
{
    struct ui_type
    {
        QLabel* m_editPath;
        QPushButton* m_btnNew;
        QPushButton* m_btnOpen;
        QPushButton* m_btnAbout;

        void SetupUI(QWidget* pView);
        void RetranslateUI(QWidget* pView);
    };
	

public:

	ToolBar(QWidget * parent = nullptr);

    const ui_type& UI() const { return (_ui); }

private:
	
    ui_type _ui;
};

#endif // !_bnb_Navigation_View_H_
