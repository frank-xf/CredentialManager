#ifndef _bnb_Edit_Dialog_H_
#define _bnb_Edit_Dialog_H_

QT_BEGIN_NAMESPACE

class QLabel;
class QLineEdit;
class QPushButton;

//------------------------------------------------------------------------------

template<typename _Ty, unsigned int n>
class EditDialog : public QDialog
{
    static_assert(0 < n, R"(the template parameter 'n' must be greater than 0)");

protected:

    using base_type = EditDialog;

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
            ui_utils::SetBackgroundColor(pView, Qt::white);

            m_labHint = ui_utils::MakeDynamicLabel(pView, Qt::red, Qt::AlignCenter);

            CreateLabel(pView);

            m_btnOK = ui_utils::MakeButton(pView);
            m_btnCancel = ui_utils::MakeButton(pView);

            m_btnOK->setDefault(true);

            QVBoxLayout* pvLayout1 = new QVBoxLayout;
            QVBoxLayout* pvLayout2 = new QVBoxLayout;
            pvLayout1->setMargin(0);
            pvLayout2->setMargin(0);
            pvLayout1->setSpacing(2);
            pvLayout2->setSpacing(2);
            for (unsigned int i = 0; i < n; ++i)
            {
                pvLayout1->addWidget(_labText[i]);
                pvLayout2->addWidget(m_editText[i]);
            }

            QHBoxLayout* phLayout1 = new QHBoxLayout;
            phLayout1->setMargin(0);
            phLayout1->setSpacing(0);
            phLayout1->addLayout(pvLayout1);
            phLayout1->addLayout(pvLayout2);
            phLayout1->addLayout(LayoutLayoutMarkLabel(pView));

            QHBoxLayout* phLayout2 = ui_utils::MakeButtonLayout(m_btnOK, m_btnCancel);

            QVBoxLayout* pMainLayout = new QVBoxLayout;
            pMainLayout->setMargin(4);
            pMainLayout->setSpacing(4);
            pMainLayout->addWidget(m_labHint);
            pMainLayout->addLayout(phLayout1);
            pMainLayout->addLayout(phLayout2);

            RetranslateUI(pView);

            pView->setLayout(pMainLayout);
            pView->setFixedSize(pView->sizeHint());
        }

        void RetranslateUI(EditDialog* pView)
        {
            RetranslateLabel(pView);

            m_btnOK->setText("OK");
            m_btnCancel->setText("Cancel");
        }

        void CreateLabel(EditDialog* pView)
        {
            for (unsigned int i = 0; i < n; ++i)
            {
                _labText[i] = ui_utils::MakeStaticLabel(pView);
                m_editText[i] = ui_utils::MakeLineEdit(pView);
            }
        }

        QVBoxLayout* LayoutLayoutMarkLabel(EditDialog* pView)
        {
            return new QVBoxLayout;
        }

        void RetranslateLabel(EditDialog* pView)
        {
            for (unsigned int i = 0; i < n; ++i)
                _labText[i]->setText("Label: ");
        }
    };

    EditDialog(QWidget * parent, Qt::WindowFlags flags) : QDialog(parent, flags)
    {
        _ui.SetupUI(this);

        QObject::connect(_ui.m_btnOK, &QPushButton::clicked, [this]() { OnClickedOK(); });
        QObject::connect(_ui.m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
        for (unsigned int i = 0; i < n; ++i)
            QObject::connect(_ui.m_editText[i], &QLineEdit::textEdited, [this](const QString strText) { _ui.m_labHint->clear(); });
    }

    virtual void OnClickedOK() { accept(); }

    ui_type _ui;

};	// class EditDialog

//------------------------------------------------------------------------------

class EditCredentialDialog : public EditDialog<EditCredentialDialog, 2>
{
public:

    EditCredentialDialog(bnb::Credential& pc, QWidget* parent = nullptr);

private:

    void OnClickedOK() override;

private:

    bnb::Credential& m_Credential;

};  // class EditCredentialDialog

    //------------------------------------------------------------------------------

class EditPasswordDialog : public EditDialog<EditPasswordDialog, 3>
{
public:

    EditPasswordDialog(bnb::Credential& pc, QWidget* parent = nullptr);

private:

    void OnClickedOK() override;

private:

    bnb::Credential& m_Credential;

};  // class EditPasswordDialog

//------------------------------------------------------------------------------

class EditPlatformDialog : public EditDialog<EditPlatformDialog, 3>
{
public:

    EditPlatformDialog(bnb::Credential& pc, const bnb::platform_node* pp, QWidget* parent);
    const bnb::platform_node* GetPlatform() const;

private:

    void OnClickedOK() override;

private:

    bnb::Credential& m_Credential;
    const bnb::platform_node* m_Platform;

};  // class EditPlatformDialog

//------------------------------------------------------------------------------

class EditAccountDialog : public EditDialog<EditAccountDialog, 3>
{
public:

    EditAccountDialog(bnb::platform_node& pp, const bnb::account_node* pa, QWidget * parent);
    const bnb::account_node* GetAccount() const;

private:

    void OnClickedOK() override;

private:

    bnb::platform_node& m_Platform;
    const bnb::account_node* m_Account;

};  // class EditAccountDialog

//------------------------------------------------------------------------------

class EditPairDialog : public EditDialog<EditPairDialog, 3>
{
public:

    EditPairDialog(bnb::account_node& pa, const bnb::pair_node* pp, QWidget * parent);
    const bnb::pair_node* GetPair() const;

private:

    void OnClickedOK() override;

private:

    bnb::account_node& m_Account;
    const bnb::pair_node* m_Pair;

};  // class EditPairDialog

QT_END_NAMESPACE

#endif  // _bnb_Edit_Dialog_H_
