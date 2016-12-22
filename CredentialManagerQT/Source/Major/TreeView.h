#ifndef _bnb_Tree_View_H_
#define _bnb_Tree_View_H_

QT_BEGIN_NAMESPACE

class TreeView : public QTreeWidget
{
private:

    struct ui_type
    {
        QAction* m_actAddAccount;
        QAction* m_actAddPlatform;
        QAction* m_actAddPair;
        QAction* m_actDelAccount;
        QAction* m_actDelPlatform;
        QAction* m_actDelPair;
        QAction* m_actEditAccount;
        QAction* m_actEditPlatform;
        QAction* m_actEditPair;
        QAction* m_actModifyPassword;
        QAction* m_actEditCredential;
        QAction* m_actDelCredential;

        void SetupUI(TreeView* pView);
        void RetranslateUI(TreeView* pView);
    };

public:

	TreeView(DelegateMainView* pDelegate = nullptr, QWidget * parent = nullptr);

	void ClearCredential();

    QTreeWidgetItem* AddCredential(const bnb::Credential& credential);
    QTreeWidgetItem* AddPlatform(const bnb::platform_node& pp);
	QTreeWidgetItem* AddAccount(const bnb::account_node& pa);
	QTreeWidgetItem* AddPair(const bnb::pair_node& pp);

    QTreeWidgetItem* UpdateCredential(const bnb::Credential& pc);
    QTreeWidgetItem* UpdatePlatform(const bnb::platform_node& pp);
    QTreeWidgetItem* UpdateAccount(const bnb::account_node& pa);
    QTreeWidgetItem* UpdatePair(const bnb::pair_node& pp);

    QTreeWidgetItem* MovePlatform(unsigned int id1, unsigned int id2, int offset);
    QTreeWidgetItem* MoveAccount(unsigned int id1, unsigned int id2, unsigned int id3, int offset);
    QTreeWidgetItem* MovePair(unsigned int id1, unsigned int id2, unsigned int id3, unsigned int id4, int offset);

    bool RemoveCredential(unsigned int id1);
    bool RemovePlatform(unsigned int id1, unsigned int id2);
    bool RemoveAccount(unsigned int id1, unsigned int id2, unsigned int id3);
    bool RemovePair(unsigned int id1, unsigned int id2, unsigned int id3, unsigned int id4);

    bool Reschedule(const bnb::Credential& credential);
    bool Reschedule(const bnb::platform_node& pp);
    bool Reschedule(const bnb::account_node& pa);

    QTreeWidgetItem* FindItem(unsigned int id);
    QTreeWidgetItem* FindItem(QTreeWidgetItem* parent, unsigned int id);

private:

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void OnTreeContextMenu(const QPoint& pos);
    void OnItemChanged(QTreeWidgetItem *cur, QTreeWidgetItem *pre);

    void OnAddPlatform();
    void OnAddAccount();
    void OnAddPair();
    void OnEditPlatform();
    void OnEditAccount();
    void OnEditPair();
    void OnEditCredential();
    void OnMotifyPassword();
    void OnRemovePlatform();
    void OnRemoveAccount();
	void OnRemovePair();
	void OnRemoveCredential();

    QTreeWidgetItem* _AddCredential(const bnb::Credential & credential);
    QTreeWidgetItem* _AddPlatform(QTreeWidgetItem * parent, const bnb::platform_node& pp);
    QTreeWidgetItem* _AddAccount(QTreeWidgetItem * parent, const bnb::account_node& pa);
    QTreeWidgetItem* _AddPair(QTreeWidgetItem * parent, const bnb::pair_node& pp);

    void _EditCredential(QTreeWidgetItem* item_credential);
    void _EditPlatform(QTreeWidgetItem* item_platform);
    void _EditAccount(QTreeWidgetItem* item_account);
    void _EditPair(QTreeWidgetItem* item_pair);

private:

    ui_type _ui;

    DelegateMainView* _delegate;
};

QT_END_NAMESPACE

#endif	// _bnb_Tree_View_H_
