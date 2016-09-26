#ifndef _bnb_View_Base_H_
#define _bnb_View_Base_H_

class QTableWidget;

class ViewBase : public QWidget {
	
protected:

    struct ui_type
    {
        QTableWidget* m_tabView;

        void SetupUI(QWidget* pView);
        void RetranslateUI(QWidget* pView);
    };

public:

	ViewBase(QWidget * parent = nullptr);
    ~ViewBase() = 0 { }

};


#endif  // _bnb_View_Base_H_