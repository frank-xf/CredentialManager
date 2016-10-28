#ifndef _bnb_Credential_View_H_
#define _bnb_Credential_View_H_

class CredentialView : public ViewBase
{

	struct ui_type : public ViewBase::ui_type
	{
	private:

		QLabel* _labUpdateTime;
		QLabel* _labUserName;
		QLabel* _labDisplay;

	public:

		QLabel* m_labUpdateTime;
		QLabel* m_labUserName;
		QLabel* m_labDisplay;

		void SetupUI(QWidget* pView);
		void RetranslateUI(QWidget* pView);
	};

public:

	CredentialView(QWidget * parent = nullptr);

private:

	ui_type _ui;
};

#endif  // _bnb_Credential_View_H_