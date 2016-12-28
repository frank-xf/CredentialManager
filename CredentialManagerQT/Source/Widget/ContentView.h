#ifndef _bnb_Content_View_H_
#define _bnb_Content_View_H_

QT_BEGIN_NAMESPACE

class QLineEdit;
class DropTable;

//------------------------------------------------------------------------------

class BaseView : public QWidget
{
protected:

    using id_type = bnb::Credential::id_type;

    const id_type m_ID;

    BaseView(id_type id, QWidget * parent = nullptr)
        : QWidget(parent)
        , m_ID(id)
    { }

public:

    id_type GetID() const { return m_ID; }

};	// class BaseView

//------------------------------------------------------------------------------

template<typename _Ty, unsigned int n>
class ContentView : public BaseView, public DropTable::delegate_type
{
    static_assert(0 < n, R"(the template parameter 'n' must be greater than 0)");

protected:

    using base_type = ContentView;

    struct ui_type
    {
    private:

        QLabel* _labText[n];

    public:

        QPushButton* m_btnBack;
		QPushButton* m_btnEdit;
		QPushButton* m_btnRemove;
        QLineEdit* m_editText[n];
        DropTable* m_tabView;

        void SetupUI(ContentView* pView)
        {
            m_btnBack = ui_utils::MakeTransparentButton(pView);
            m_btnEdit = ui_utils::MakeTransparentButton(pView);
            m_btnRemove = ui_utils::MakeTransparentButton(pView);

            CreateLabel(pView);

            m_tabView = new DropTable(pView, pView);

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
			phLayout1->setSpacing(8);
            phLayout1->addStretch(1);
            phLayout1->addWidget(m_btnBack);
			phLayout1->addWidget(m_btnEdit);
			phLayout1->addWidget(m_btnRemove);

            QHBoxLayout* phLayout2 = new QHBoxLayout;
			phLayout2->setContentsMargins(0, 0, 0, 16);
			phLayout2->setSpacing(0);
			phLayout2->addLayout(pvLayout1);
			phLayout2->addLayout(pvLayout2);

            QVBoxLayout* pvMainLayout = new QVBoxLayout;
            pvMainLayout->setMargin(0);
            pvMainLayout->setSpacing(0);
			pvMainLayout->addLayout(phLayout1);
			pvMainLayout->addLayout(phLayout2);
            pvMainLayout->addWidget(m_tabView, 1);

            QHBoxLayout* phMainLayout = new QHBoxLayout;
            phMainLayout->setContentsMargins(4, 0, 4, 4);
            phMainLayout->setSpacing(0);
            phMainLayout->addStretch(1);
            phMainLayout->addLayout(pvMainLayout, 8);
            phMainLayout->addStretch(1);

            pView->setLayout(phMainLayout);

            m_btnBack->setText("<-- ");
            m_btnEdit->setText("Edit");
            m_btnRemove->setText("Remove");

            RetranslateUI(pView);
        }

        void RetranslateUI(ContentView* pView) { }

        void CreateLabel(ContentView* pView)
        {
            for (unsigned int i = 0; i < n; ++i)
            {
                _labText[i] = ui_utils::MakeStaticLabel(pView, ui_utils::g_clrLabel);
                m_editText[i] = ui_utils::MakeShowLine(pView);
            }
        }
    };

    ContentView(id_type id, DelegateType* pDelegate, QWidget * parent = nullptr)
        : BaseView(id, parent)
        , _delegate(pDelegate)
    {
        _ui.SetupUI(this);

        QObject::connect(_ui.m_btnBack, &QPushButton::clicked, [this]() { OnClickedBack(); });
        QObject::connect(_ui.m_btnEdit, &QPushButton::clicked, [this]() { OnClickedEdit(); });
		QObject::connect(_ui.m_btnRemove, &QPushButton::clicked, [this]() { OnClickedRemove(); });
    }

	virtual void OnClickedBack() { }
    virtual void OnClickedEdit() { }
    virtual void OnClickedRemove() { }

    ui_type _ui;

    DelegateType* _delegate;

};	// class ContentView

//------------------------------------------------------------------------------

class CredentialView : public ContentView<CredentialView, 3>
{
public:

    CredentialView(const bnb::Credential& credential, DelegateType* pDelegate, QWidget * parent = nullptr);

    void UpdateInfo();
    void UpdateTable();
    void UpdateTable(id_type id);

private:

    void OnAdd() override;
    void OnEdit(unsigned int id) override;
    void OnRemove(unsigned int id) override;
    void OnMove(unsigned int id, int offset) override;
    void OnSort(int cln, bool ascending) override;
    void OnDoubleClicked(unsigned int id) override;

	void OnClickedEdit() override;
	void OnClickedRemove() override;

private:

    const bnb::Credential& m_Credential;

};	// class CredentialView

//------------------------------------------------------------------------------

class PlatformView : public ContentView<PlatformView, 3>
{
public:

    PlatformView(const bnb::platform_node& tp, DelegateType* pDelegate, QWidget * parent = nullptr);

    void UpdateInfo();
    void UpdateTable();
    void UpdateTable(id_type id);

private:

    void OnAdd() override;
    void OnEdit(unsigned int id) override;
    void OnRemove(unsigned int id) override;
    void OnMove(unsigned int id, int offset) override;
	void OnSort(int cln, bool ascending) override;
    void OnDoubleClicked(unsigned int id) override;

    void OnClickedBack() override;
    void OnClickedEdit() override;
	void OnClickedRemove() override;

private:

    const bnb::platform_node& m_Platform;

};	// class PlatformView

//------------------------------------------------------------------------------

class AccountView : public ContentView<AccountView, 2>
{
public:

    AccountView(const bnb::account_node& tp, DelegateType* pDelegate, QWidget * parent = nullptr);

    void UpdateInfo();
    void UpdateTable();
    void UpdateTable(id_type id);

private:

    void OnAdd() override;
    void OnEdit(unsigned int id) override;
    void OnRemove(unsigned int id) override;
	void OnMove(unsigned int id, int offset) override;
	void OnSort(int cln, bool ascending) override;
    void OnDoubleClicked(unsigned int id) override;

    void OnClickedBack() override;
	void OnClickedEdit() override;
	void OnClickedRemove() override;

private:

    const bnb::account_node& m_Account;

};	// class AccountView

QT_END_NAMESPACE

#endif  // _bnb_Content_View_H_
