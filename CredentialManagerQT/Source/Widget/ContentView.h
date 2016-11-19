#ifndef _bnb_Content_View_H_
#define _bnb_Content_View_H_

QT_BEGIN_NAMESPACE

class QLineEdit;
class QTableWidget;

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

        QWidget* _viewCentral;
        QLabel* _labText[n];

    public:

        QLineEdit* m_editText[n];

        QTableWidget* m_tabView;

        void SetupUI(QWidget* pView)
        {
            _viewCentral = new QWidget(pView);
            _viewCentral->setObjectName("_viewCentral");
            _viewCentral->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
            
            CreateLabel();

            m_tabView = new QTableWidget(_viewCentral);
            m_tabView->setItemDelegate(new NoFocusDelegate);
            m_tabView->setMinimumSize(512, 128);
            m_tabView->setShowGrid(true);
            m_tabView->setContextMenuPolicy(Qt::CustomContextMenu);
            m_tabView->setSelectionMode(QAbstractItemView::SingleSelection);
            m_tabView->setSelectionBehavior(QAbstractItemView::SelectRows);
            m_tabView->setEditTriggers(QAbstractItemView::NoEditTriggers);
            m_tabView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
            m_tabView->verticalHeader()->setDefaultSectionSize(ui_utils::def_widget_h);
            m_tabView->verticalHeader()->setVisible(false);
            m_tabView->horizontalHeader()->setFixedHeight(ui_utils::def_widget_h);
            m_tabView->horizontalHeader()->setSectionsClickable(false);
            m_tabView->horizontalHeader()->setHighlightSections(false);
            m_tabView->horizontalHeader()->setStretchLastSection(true);
            m_tabView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
            m_tabView->horizontalHeader()->setStyleSheet("QHeaderView::section{ background-color:#F0F0F0; color: #FF4080; }");

            QFont font = QGuiApplication::font();
            font.setPointSize(ui_utils::def_text_size);
            m_tabView->horizontalHeader()->setFont(font);

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

            _viewCentral->setLayout(pvMainLayout);

            QHBoxLayout* phMainLayout = new QHBoxLayout;
            phMainLayout->setMargin(4);
            phMainLayout->setSpacing(0);
            phMainLayout->addStretch(1);
            phMainLayout->addWidget(_viewCentral, 2);
            phMainLayout->addStretch(1);

            pView->setLayout(phMainLayout);

            RetranslateUI(pView);
        }

        void RetranslateUI(QWidget* pView) { }

        void CreateLabel()
        {
            for (unsigned int i = 0; i < n; ++i)
            {
                _labText[i] = ui_utils::MakeStaticLabel(_viewCentral, ui_utils::lab_text_w, ui_utils::g_clrLabel);
                m_editText[i] = ui_utils::MakeShowLine(_viewCentral);
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

    PlatformView(const bnb::platform_tree::data_type& tp, QWidget * parent = nullptr);

    void UpdateInfo();
    void UpdateTable();
    void UpdateTable(unsigned int id);

private:

    const bnb::platform_tree::data_type& m_Platform;

};	// class PlatformView

    //------------------------------------------------------------------------------

class AccountView : public ContentView<AccountView, 2>
{
public:

    AccountView(const bnb::account_tree::data_type& tp, QWidget * parent = nullptr);

    void UpdateInfo();
    void UpdateTable();
    void UpdateTable(unsigned int id);

private:

    const bnb::account_tree::data_type& m_Account;

};	// class AccountView

QT_END_NAMESPACE

#endif  // _bnb_Content_View_H_
