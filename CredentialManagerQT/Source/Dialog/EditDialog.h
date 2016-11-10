#ifndef _bnb_Edit_Dialog_H_
#define _bnb_Edit_Dialog_H_

QT_BEGIN_NAMESPACE

class QLabel;
class QLineEdit;
class QPushButton;

//------------------------------------------------------------------------------

template<unsigned int n>
class EditDialog : public QDialog
{
	static_assert(0 < n, "the template parameter n must be greater than 0 ");

	struct ui_type
	{
	private:

		QLabel* _labText[n];

	public:

		QLabel* m_labHint;

		QPushButton* m_btnOK;
		QPushButton* m_btnCancel;

		QLineEdit* m_editText[n];

		void SetupUI(EditDialog* pView)
		{
			pView->setObjectName(typeid(decltype(*pView)).name());
			pView->setFixedSize(ui_utils::dlg_username_w, ui_utils::dlg_username_h);

			m_labHint = new QLabel(pView);
			m_labHint->setAlignment(Qt::AlignCenter);
			m_labHint->setFixedHeight(20);

			m_btnOK = new QPushButton(pView);
			m_btnCancel = new QPushButton(pView);

			QVBoxLayout* pMainLayout = new QVBoxLayout;
			pMainLayout->setMargin(4);
			pMainLayout->setSpacing(2);
			pMainLayout->addWidget(m_labHint);

			for (unsigned int i = 0; i < n; ++i)
			{
				_labText[i] = ui_utils::MakeStaticLabel(pView, ui_utils::dlg_username_w, ui_utils::dlg_username_h, Qt::black, 12);
				m_editText[i] = new QLineEdit(pView);
				m_editText[i]->setFixedSize(ui_utils::edit_default_w, ui_utils::edit_default_h);

				QHBoxLayout* phLayout = new QHBoxLayout;
				phLayout->setMargin(0);
				phLayout->setSpacing(2);
				phLayout->addWidget(_labText[i]);
				phLayout->addWidget(m_editText[i], 1);
				if (0 == i)
				{
					m_editText[0]->setMaxLength(64);
					phLayout->addWidget(ui_utils::MakeMarkLabel(pView));
				}

				pMainLayout->addLayout(phLayout);
			}

			QHBoxLayout* phLayout = new QHBoxLayout;
			phLayout->setContentsMargins(8, 20, 8, 8);
			phLayout->addStretch(1);
			phLayout->addWidget(m_btnOK);
			phLayout->addStretch(1);
			phLayout->addWidget(m_btnCancel);
			phLayout->addStretch(1);

			pMainLayout->addLayout(phLayout, 1);

			pView->setLayout(pMainLayout);

			RetranslateUI(pView);
		}

		void RetranslateUI(EditCredentialDialog* pView);
	};

public:

	EditDialog(QWidget * parent, Qt::WindowFlags flags) : QDialog(parent, flags)
	{
		_ui.SetupUI(this);
		
		QObject::connect(_ui.m_btnOK, &QPushButton::clicked, this, &EditDialog::OnClickedOK);
		QObject::connect(_ui.m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
		for (unsigned int i = 0; i < n; ++i)
			QObject::connect(_ui.m_editText[i], &QLineEdit::textEdited, this, &EditDialog::OnChangedText);
	}

	void OnChangedText(const QString &)	{ _ui.m_labHint->clear(); }

	void OnClickedOK() { accept(); }

protected:

	ui_type _ui;
};

//------------------------------------------------------------------------------

class EditCredentialDialog : public QDialog
{
    struct ui_type
    {
    private:

		QLabel* _labUserName;
		QLabel* _labDisplay;

    public:

        QLabel* m_labHint;
		QLineEdit* m_editUserName;
		QLineEdit* m_editDisplay;

        QPushButton* m_btnOK;
        QPushButton* m_btnCancel;

        void SetupUI(EditCredentialDialog* pView);
        void RetranslateUI(EditCredentialDialog* pView);
    };

public:

    EditCredentialDialog(bnb::Credential* credential, QWidget* parent = nullptr);

private:

    void OnChangedText(const QString&);
    void OnClickedOK();
    
private:

    ui_type _ui;
	bnb::Credential* m_Credential;

};  // class EditUserNameDialog

//------------------------------------------------------------------------------

class EditPasswordDialog : public QDialog
{
    struct ui_type
    {
    private:

        QLabel* _labOldPassword;
        QLabel* _labNewPassword;
        QLabel* _labValidate;

    public:

        QLabel* m_labHint;
        QLineEdit* m_editOldPassword;
        QLineEdit* m_editNewPassword;
        QLineEdit* m_editValidate;

        QPushButton* m_btnOK;
        QPushButton* m_btnCancel;

        void SetupUI(EditPasswordDialog* pView);
        void RetranslateUI(EditPasswordDialog* pView);
    };

public:

    EditPasswordDialog(QWidget* parent = nullptr);

private:

    void OnChangedText(const QString&);
    void OnClickedOK();

private:

    ui_type _ui;

};  // class EditPasswordDialog

//------------------------------------------------------------------------------

class EditPlatformDialog : public QDialog
{
    struct ui_type
    {
    private:

        QLabel* _labPlatform;
        QLabel* _labUrl;
        QLabel* _labDisplay;

    public:

        QLabel* m_labHint;

        QLineEdit* m_editPlatform;
        QLineEdit* m_editUrl;
        QLineEdit* m_editDisplay;

        QPushButton* m_btnOK;
        QPushButton* m_btnCancel;

        void SetupUI(EditPlatformDialog* pView);
        void RetranslateUI(EditPlatformDialog* pView);
    };

public:

	EditPlatformDialog(bnb::platform_type* platform, QWidget* parent);

private:

    void OnChangedText(const QString&);
    void OnClickedOK();

private:
	
    ui_type _ui;

    bnb::platform_type* m_Platform;

};  // class EditPlatformDialog

//------------------------------------------------------------------------------

class EditAccountDialog : public QDialog
{
    struct ui_type
    {
    private:

        QLabel* _labPlatform;
        QLabel* _labAccount;
        QLabel* _labDisplay;

    public:

        QLabel* m_labHint;

        QLineEdit* m_editPlatform;
        QLineEdit* m_editAccount;
        QLineEdit* m_editDisplay;

        QPushButton* m_btnOK;
        QPushButton* m_btnCancel;

        void SetupUI(EditAccountDialog* pView);
        void RetranslateUI(EditAccountDialog* pView);
    };

public:

    EditAccountDialog(bnb::platform_type* pp, bnb::account_type* pa, QWidget * parent);
	const bnb::account_type* GetAccount() const;


    void OnChangedText(const QString&);
    void OnClickedOK();

private:

    ui_type _ui;

    bnb::account_type* m_Account;
    bnb::platform_type* m_Platform;

};  // class EditAccountDialog

//------------------------------------------------------------------------------

class EditPropertyDialog : public QDialog
{

};  // class EditPropertyDialog

QT_END_NAMESPACE

#endif  // _bnb_Edit_Dialog_H_
