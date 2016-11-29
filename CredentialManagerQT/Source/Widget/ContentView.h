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

            QVBoxLayout* pvLayout = new QVBoxLayout;
            pvLayout->setMargin(0);
            pvLayout->setSpacing(0);

            for (unsigned int i = 0; i < n; ++i)
            {
                QHBoxLayout* phLayout = new QHBoxLayout;
                phLayout->setMargin(0);
                phLayout->setSpacing(0);
                phLayout->addWidget(_labText[i]);
                phLayout->addWidget(m_editText[i], 1);

                pvLayout->addLayout(phLayout);
            }

            QVBoxLayout* pvMainLayout = new QVBoxLayout;
            pvMainLayout->setMargin(0);
            pvMainLayout->setSpacing(16);
            pvMainLayout->addLayout(pvLayout);
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
                _labText[i] = ui_utils::MakeStaticLabel(pView, ui_utils::lab_text_w, ui_utils::g_clrLabel);
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

    CredentialView(bnb::Credential& credential, QWidget * parent = nullptr);

    void UpdateInfo();
    void UpdateTable();
    void UpdateTable(unsigned int id);

private:

    bnb::Credential& m_Credential;

};	// class CredentialView

//------------------------------------------------------------------------------

class PlatformView : public ContentView<PlatformView, 3>
{
public:

    PlatformView(const bnb::platform_list::data_type& tp, QWidget * parent = nullptr);

    void UpdateInfo();
    void UpdateTable();
    void UpdateTable(unsigned int id);

private:

    const bnb::platform_list::data_type& m_Platform;

};	// class PlatformView

//------------------------------------------------------------------------------

class AccountView : public ContentView<AccountView, 2>
{
public:

    AccountView(const bnb::account_list::data_type& tp, QWidget * parent = nullptr);

    void UpdateInfo();
    void UpdateTable();
    void UpdateTable(unsigned int id);

private:

    const bnb::account_list::data_type& m_Account;

};	// class AccountView

QT_END_NAMESPACE

#endif  // _bnb_Content_View_H_
