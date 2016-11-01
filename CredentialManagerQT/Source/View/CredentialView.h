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

	CredentialView(bnb::Credential& credential, QWidget * parent = nullptr);

	void UpdateInfo();
	void UpdateTable();

private:

	ui_type _ui;

	bnb::Credential& m_Credential;
};

#endif  // _bnb_Credential_View_H_