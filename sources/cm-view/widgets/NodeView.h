#pragma once

QT_BEGIN_NAMESPACE

class QLineEdit;
class DropTable;

//------------------------------------------------------------------------------

class NodeView : public QWidget
{
public:

    struct ui_type
    {
    private:

        static constexpr unsigned int _IndicatorSize = static_cast<unsigned int>(xf::credential::credential_type::ct_max) - 1;
        static constexpr unsigned int _FieldMaxSize = 4;

    public:

        std::pair<QLabel*, QPushButton*> m_Indicators[_IndicatorSize];
        QList<std::pair<QLabel*, QLineEdit*>> m_Displays[_FieldMaxSize];

		QPushButton* m_btnEdit;
		QPushButton* m_btnRemove;
        DropTable* m_tabView;

        void SetupUI(NodeView* pView);
        void RetranslateUI(NodeView* pView);
        void Adjust(unsigned int nIndicator, unsigned int nDisplay);
    };

    NodeView(QWidget* parent);

    void Show(const xf::credential::credential_t& credential);
    void Show(const xf::credential::platform_t& platform);
    void Show(const xf::credential::account_t& account);

    void OnClickedEdit();
    void OnClickedRemove();

private:

    ui_type _ui;

};	// class NodeView

//------------------------------------------------------------------------------

class CredentialView : public NodeView<CredentialView, 3>
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

class PlatformView : public NodeView<PlatformView, 3>
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

class AccountView : public NodeView<AccountView, 2>
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
