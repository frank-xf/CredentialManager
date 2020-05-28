#pragma once

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

	TreeView(QWidget * parent = nullptr);

	void ClearCredential();
    QTreeWidgetItem* InitCredential(const xf::credential::credential_t& credential);

    QTreeWidgetItem* AddPlatform(QTreeWidgetItem* pItem, const xf::credential::platform_t& platform);
	QTreeWidgetItem* AddAccount(QTreeWidgetItem* pItem, const xf::credential::account_t& account);
    /*
    QTreeWidgetItem* UpdateCredential(const bnb::Credential& credential);
    QTreeWidgetItem* UpdatePlatform(const bnb::platform_node& platform);
    QTreeWidgetItem* UpdateAccount(const bnb::account_node& account);
    QTreeWidgetItem* UpdatePair(const bnb::pair_node& pair);

    QTreeWidgetItem* MovePlatform(id_type credentialId, id_type platformId, int offset);
    QTreeWidgetItem* MoveAccount(id_type credentialId, id_type platformId, id_type accountId, int offset);
    QTreeWidgetItem* MovePair(id_type credentialId, id_type platformId, id_type accountId, id_type pairId, int offset);

    bool RemoveCredential(id_type credentialId);
    bool RemovePlatform(id_type credentialId, id_type platformId);
    bool RemoveAccount(id_type credentialId, id_type platformId, id_type accountId);
    bool RemovePair(id_type credentialId, id_type platformId, id_type accountId, id_type pairId);

    bool Reschedule(const bnb::Credential& credential);
    bool Reschedule(const bnb::platform_node& platform);
    bool Reschedule(const bnb::account_node& account);

    QTreeWidgetItem* FindItem(id_type id);

    template<typename _Ty1, typename ... _Ty2>
    QTreeWidgetItem* FindItem(_Ty1 id, _Ty2 ... ids)
    {
        static_assert(std::is_same<_Ty1, id_type>::value, "The type of parameter must be id_type.");

        return (_FindItem(FindItem(id), ids ...));
    }

    template<typename ... _Ty>
    bool SwitchToNode(_Ty ... ids)
    {
        if (auto pItem = FindItem(ids ...))
        {
            setCurrentItem(pItem);
            return true;
        }

        return false;
    }
    */
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
    /*
    QTreeWidgetItem* _AddCredential(const bnb::Credential & credential);
    QTreeWidgetItem* _AddPlatform(QTreeWidgetItem * parent, const bnb::platform_node& platform);
    QTreeWidgetItem* _AddAccount(QTreeWidgetItem * parent, const bnb::account_node& account);
    QTreeWidgetItem* _AddPair(QTreeWidgetItem * parent, const bnb::pair_node& pair);
    */
    void _EditCredential(QTreeWidgetItem* item_credential);
    void _EditPlatform(QTreeWidgetItem* item_platform);
    void _EditAccount(QTreeWidgetItem* item_account);
    void _EditPair(QTreeWidgetItem* item_pair);

    // QTreeWidgetItem* _FindItem(QTreeWidgetItem * parent, id_type id);

    template<typename _Ty1, typename ... _Ty2>
    QTreeWidgetItem* _FindItem(QTreeWidgetItem* parent, _Ty1 id, _Ty2 ... ids)
    {/*
        static_assert(std::is_same<_Ty1, id_type>::value, "The type of parameter must be id_type.");

        if (parent) return (_FindItem(_FindItem(parent, id), ids ...));
        */
        return nullptr;
    }

private:

    ui_type _ui;
};

QT_END_NAMESPACE
