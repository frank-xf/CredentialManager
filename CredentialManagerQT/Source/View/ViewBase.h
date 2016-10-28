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

	ViewBase(unsigned int id, QWidget * parent = nullptr);
    ~ViewBase() = 0 { }

	unsigned int GetID() const { return m_ID; }

private:

	const unsigned int m_ID;

};


#endif  // _bnb_View_Base_H_