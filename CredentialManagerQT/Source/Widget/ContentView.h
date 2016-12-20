#ifndef _bnb_Content_View_H_
#define _bnb_Content_View_H_

QT_BEGIN_NAMESPACE

class QLineEdit;
class DropTable;

//------------------------------------------------------------------------------

class BaseView : public QWidget
{
protected:

    const unsigned int m_ID;

    BaseView(unsigned int id, QWidget * parent = nullptr)
        : QWidget(parent)
        , m_ID(id)
    { }

public:

    unsigned int GetID() const { return m_ID; }

};	// class BaseView

//------------------------------------------------------------------------------

template<typename _Ty, unsigned int n>
class ContentView : public BaseView
{
    static_assert(0 < n, R"(the template parameter 'n' must be greater than 0)");

protected:

    using base_type = ContentView;

    struct ui_type
    {
    private:

        QLabel* _labText[n];

    public:

        QLineEdit* m_editText[n];

        DropTable* m_tabView;

        void SetupUI(ContentView* pView)
        {
            CreateLabel(pView);

            m_tabView = new DropTable(pView);

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

            QHBoxLayout* phLayout = new QHBoxLayout;
            phLayout->setMargin(0);
            phLayout->setSpacing(0);
            phLayout->addLayout(pvLayout1);
            phLayout->addLayout(pvLayout2);

            QVBoxLayout* pvMainLayout = new QVBoxLayout;
            pvMainLayout->setMargin(0);
            pvMainLayout->setSpacing(16);
            pvMainLayout->addLayout(phLayout);
            pvMainLayout->addWidget(m_tabView, 1);

            QHBoxLayout* phMainLayout = new QHBoxLayout;
            phMainLayout->setMargin(4);
            phMainLayout->setSpacing(0);
            phMainLayout->addStretch(1);
            phMainLayout->addLayout(pvMainLayout, 2);
            phMainLayout->addStretch(1);

            pView->setLayout(phMainLayout);

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

    ContentView(unsigned int id, QWidget * parent = nullptr)
        : BaseView(id, parent)
    {
        _ui.SetupUI(this);
    }

    ui_type _ui;

};	// class ContentView

//------------------------------------------------------------------------------

class CredentialView : public ContentView<CredentialView, 3>
{
public:

    CredentialView(const bnb::Credential& credential, QWidget * parent = nullptr);

    void UpdateInfo();
    void UpdateTable();
    void UpdateTable(unsigned int id);

private:

    const bnb::Credential& m_Credential;

};	// class CredentialView

//------------------------------------------------------------------------------

class PlatformView : public ContentView<PlatformView, 3>
{
public:

    PlatformView(const bnb::platform_node& tp, QWidget * parent = nullptr);

    void UpdateInfo();
    void UpdateTable();
    void UpdateTable(unsigned int id);

private:

    const bnb::platform_node& m_Platform;

};	// class PlatformView

//------------------------------------------------------------------------------

class AccountView : public ContentView<AccountView, 2>
{
public:

    AccountView(const bnb::account_node& tp, QWidget * parent = nullptr);

    void UpdateInfo();
    void UpdateTable();
    void UpdateTable(unsigned int id);

private:

    const bnb::account_node& m_Account;

};	// class AccountView

QT_END_NAMESPACE

#endif  // _bnb_Content_View_H_
