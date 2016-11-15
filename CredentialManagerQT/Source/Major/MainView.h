#ifndef _bnb_Credential_Main_View_H_
#define _bnb_Credential_Main_View_H_

QT_BEGIN_NAMESPACE

class QPushButton;
class QScrollArea;
class QLabel;
class ToolBar;
class StackView;
class TreeView;
class QTreeWidgetItem;

class MainView : public QWidget
{
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

        ToolBar* m_viewToolBar;
        StackView* m_viewContent;
        TreeView* m_treeView;

        void SetupUI(MainView* pView);
        void RetranslateUI(MainView* pView);
    };

public:

    MainView(QWidget *parent = nullptr);

    ui_type& UI() { return (_ui); }

private:

    void InitCredential();
    void ClearCredential();

    void OnTreeContextMenu(const QPoint& pos);
    void OnItemChanged(QTreeWidgetItem *cur, QTreeWidgetItem *pre);
    void OnDoubleClickedItem(QTreeWidgetItem *pItem, int index);

    void OnClickedNew();
    void OnClickedOpen();
    void OnClickedAbout();
    void OnEditCredential();
    void OnMotifyPassword();

    void OnAddPlatform();
    void OnAddAccount();
    void OnAddProperty();
    void OnEditPlatform();
    void OnEditAccount();
    void OnEditProperty();
    void OnRemovePlatform();
    void OnRemoveAccount();
    void OnRemoveProperty();

    bool AddPlatform();
    bool AddAccount(QTreeWidgetItem* item_platform);
    bool AddProperty(QTreeWidgetItem* item_account);
    bool EditPlatform(QTreeWidgetItem* item_platform);
    bool EditAccount(QTreeWidgetItem* item_account);
    bool EditProperty(QTreeWidgetItem* item_property);
    bool RemovePlatform(QTreeWidgetItem* item_platform);
    bool RemoveAccount(QTreeWidgetItem* item_account);
    bool RemoveProperty(QTreeWidgetItem* item_property);


private:

    ui_type _ui;

};

QT_END_NAMESPACE

#endif // _bnb_Credential_Main_View_H_
