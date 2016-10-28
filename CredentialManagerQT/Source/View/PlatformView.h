#ifndef _bnb_Platform_View_H_
#define _bnb_Platform_View_H_

class PlatformView : public ViewBase
{

	struct ui_type : public ViewBase::ui_type
	{
	private:

		QLabel* _labName;
		QLabel* _labUrl;
		QLabel* _labDisplay;

	public:

		QLabel* m_labName;
		QLabel* m_labUrl;
		QLabel* m_labDisplay;

		void SetupUI(QWidget* pView);
		void RetranslateUI(QWidget* pView);
	};


public:

	PlatformView(bnb::platform_tree::data_type& tp, QWidget * parent = nullptr);
	~PlatformView();

	void Initialize();


private:

	bnb::platform_tree::data_type& m_Platform;


};

#endif	// _bnb_Platform_View_H_
