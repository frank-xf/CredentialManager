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
        QAction* m_actAddProperty;
        QAction* m_actDelAccount;
        QAction* m_actDelPlatform;
        QAction* m_actDelProperty;
        QAction* m_actEditAccount;
        QAction* m_actEditPlatform;
        QAction* m_actEditProperty;
        QAction* m_actModifyPassword;
        QAction* m_actEditCredential;
        QAction* m_actRemoveCredential;

        void SetupUI(TreeView* pView);
        void RetranslateUI(TreeView* pView);
    };

public:

	TreeView(delegate_model_type* pDelegate = nullptr, QWidget * parent = nullptr);

	void ClearCredential();

    QTreeWidgetItem* AddCredential(const bnb::Credential& credential);
    QTreeWidgetItem* AddPlatform(const bnb::platform_node& pp);
	QTreeWidgetItem* AddAccount(const bnb::account_node& pa);
	QTreeWidgetItem* AddProperty(const bnb::property_node& pp);

    QTreeWidgetItem* UpdateCredential(unsigned int id, const bnb::Credential& pc);
    QTreeWidgetItem* UpdatePlatform(const bnb::platform_node& pp);
    QTreeWidgetItem* UpdateAccount(const bnb::account_node& pa);
    QTreeWidgetItem* UpdateProperty(const bnb::property_node& pp);

    bool RemoveCredential(unsigned int id1);
    bool RemovePlatform(unsigned int id1, unsigned int id2);
    bool RemoveAccount(unsigned int id1, unsigned int id2, unsigned int id3);
    bool RemoveProperty(unsigned int id1, unsigned int id2, unsigned int id3, unsigned int id4);

    QTreeWidgetItem* FindItem(unsigned int id);
    QTreeWidgetItem* FindItem(QTreeWidgetItem* parent, unsigned int id);

private:

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void OnTreeContextMenu(const QPoint& pos);
    void OnItemChanged(QTreeWidgetItem *cur, QTreeWidgetItem *pre);

    void OnAddPlatform();
    void OnAddAccount();
    void OnAddProperty();
    void OnEditPlatform();
    void OnEditAccount();
    void OnEditProperty();
    void OnEditCredential();
    void OnMotifyPassword();
    void OnRemovePlatform();
    void OnRemoveAccount();
    void OnRemoveProperty();

    QTreeWidgetItem* _AddCredential(const bnb::Credential & credential);
    QTreeWidgetItem* _AddPlatform(QTreeWidgetItem * parent, const bnb::platform_node& pp);
    QTreeWidgetItem* _AddAccount(QTreeWidgetItem * parent, const bnb::account_node& pa);
    QTreeWidgetItem* _AddProperty(QTreeWidgetItem * parent, const bnb::property_node& pp);

    void _EditPlatform(QTreeWidgetItem* item_platform);
    void _EditAccount(QTreeWidgetItem* item_account);
    void _EditProperty(QTreeWidgetItem* item_property);
    void _EditCredential(QTreeWidgetItem* item_credential);

private:

    ui_type _ui;

    delegate_model_type* _delegate;
};

QT_END_NAMESPACE

#endif	// _bnb_Tree_View_H_
