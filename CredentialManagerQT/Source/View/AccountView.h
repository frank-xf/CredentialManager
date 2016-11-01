#ifndef _bnb_Account_View_H_
#define _bnb_Account_View_H_


class AccountView :	public ViewBase
{
	struct ui_type : public ViewBase::ui_type
	{
	private:

		QLabel* _labName;
		QLabel* _labDisplay;

	public:

		QLabel* m_labName;
		QLabel* m_labDisplay;

		void SetupUI(QWidget* pView);
		void RetranslateUI(QWidget* pView);
	};

public:

	AccountView(bnb::account_tree::data_type& tp, QWidget * parent = nullptr);

	void UpdateInfo();
	void UpdateTable();

private:

	bnb::account_tree::data_type& m_Account;

	ui_type _ui;
};

#endif	// _bnb_Account_View_H_
