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

            m_btnOK = ui_utils::MakeButton(pView);
            m_btnCancel = ui_utils::MakeButton(pView);

            CreateLabel(pView);

            QVBoxLayout* pMainLayout = new QVBoxLayout;
            pMainLayout->setMargin(4);
            pMainLayout->setSpacing(4);
            pMainLayout->addWidget(m_labHint);

            LayoutCentral(pView, pMainLayout);

            QHBoxLayout* phLayout = new QHBoxLayout;
            phLayout->setContentsMargins(0, 12, 0, 12);
            phLayout->setSpacing(0);
            phLayout->addStretch(1);
            phLayout->addWidget(m_btnOK);
            phLayout->addStretch(1);
            phLayout->addWidget(m_btnCancel);
            phLayout->addStretch(1);

            pMainLayout->addLayout(phLayout);

            pView->setLayout(pMainLayout);
            pView->setFixedSize(pView->sizeHint());

            RetranslateUI(pView);
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

        void LayoutCentral(EditDialog* pView, QBoxLayout* pMainLayout)
        {
            for (unsigned int i = 0; i < n; ++i)
            {
                QHBoxLayout* phLayout = new QHBoxLayout;
                phLayout->setMargin(0);
                phLayout->setSpacing(0);
                phLayout->addWidget(_labText[i]);
                phLayout->addWidget(m_editText[i]);

                pMainLayout->addLayout(phLayout);
            }
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
            QObject::connect(_ui.m_editText[i], &QLineEdit::textEdited, [this](const QString strText) { OnChangedText(strText); });
    }

    virtual void OnChangedText(const QString &) { _ui.m_labHint->clear(); }
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

    EditPlatformDialog(bnb::Credential& pc, bnb::platform_tree::data_type* pp, QWidget* parent);
    const bnb::platform_tree::data_type* GetPlatform() const;

private:

    void OnClickedOK() override;

private:

    bnb::Credential& m_Credential;
    bnb::platform_tree::data_type* m_Platform;

};  // class EditPlatformDialog

    //------------------------------------------------------------------------------

class EditAccountDialog : public EditDialog<EditAccountDialog, 3>
{
public:

    EditAccountDialog(bnb::platform_tree::data_type& pp, bnb::account_tree::data_type* pa, QWidget * parent);
    const bnb::account_tree::data_type* GetAccount() const;

private:

    void OnClickedOK() override;

private:

    bnb::platform_tree::data_type& m_Platform;
    bnb::account_tree::data_type* m_Account;

};  // class EditAccountDialog

    //------------------------------------------------------------------------------

class EditPropertyDialog : public EditDialog<EditPropertyDialog, 3>
{
public:

    EditPropertyDialog(bnb::account_tree::data_type& pa, bnb::property_tree::data_type* pp, QWidget * parent);
    const bnb::property_tree::data_type* GetProperty() const;

private:

    void OnClickedOK() override;

private:

    bnb::account_tree::data_type& m_Account;
    bnb::property_tree::data_type* m_Property;

};  // class EditPropertyDialog

QT_END_NAMESPACE

#endif  // _bnb_Edit_Dialog_H_
