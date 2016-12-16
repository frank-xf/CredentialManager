#ifndef _bnb_Credential_Main_View_H_
#define _bnb_Credential_Main_View_H_

QT_BEGIN_NAMESPACE

class ToolBar;
class StackView;
class TreeView;
class QTreeWidgetItem;

class MainView : public QWidget, public delegate_model_type
{
    struct ui_type
    {
        ToolBar* m_viewToolBar;
        StackView* m_viewStack;
        TreeView* m_treeView;

        void SetupUI(MainView* pView);
        void RetranslateUI(MainView* pView);
    };

public:

    MainView(QWidget *parent = nullptr);

private:

    bool SaveCredential();
    void InitCredential();
    void ClearCredential();

    void OnClickedNew();
    void OnClickedOpen();
    void OnClickedAbout();

    bool AddPlatform(QTreeWidgetItem* item_credential);
    bool AddAccount(QTreeWidgetItem* item_platform);
    bool AddProperty(QTreeWidgetItem* item_account);
    bool EditCredential(QTreeWidgetItem* item_credential);
    bool EditPlatform(QTreeWidgetItem* item_platform);
    bool EditAccount(QTreeWidgetItem* item_account);
    bool EditProperty(QTreeWidgetItem* item_property);
    bool RemovePlatform(QTreeWidgetItem* item_platform);
    bool RemoveAccount(QTreeWidgetItem* item_account);
    bool RemoveProperty(QTreeWidgetItem* item_property);

    bool OnAddCredential() override;
    bool OnAddPlatform(unsigned int id1) override;
    bool OnAddAccount(unsigned int id1, unsigned int id2) override;
    bool OnAddProperty(unsigned int id1, unsigned int id2, unsigned int id3) override;

    bool OnUpdatePassword(unsigned int id1) override;
    bool OnUpdateCredential(unsigned int id1) override;
    bool OnUpdatePlatform(unsigned int id1, unsigned int id2) override;
    bool OnUpdateAccount(unsigned int id1, unsigned int id2, unsigned int id3) override;
    bool OnUpdateProperty(unsigned int id1, unsigned int id2, unsigned int id3, unsigned int id4) override;

    bool OnRemoveCredential(unsigned int id1) override;
    bool OnRemovePlatform(unsigned int id1, unsigned int id2) override;
    bool OnRemoveAccount(unsigned int id1, unsigned int id2, unsigned int id3) override;
    bool OnRemoveProperty(unsigned int id1, unsigned int id2, unsigned int id3, unsigned int id4) override;

private:

    ui_type _ui;

    QString m_strFile;

};

QT_END_NAMESPACE

#endif // _bnb_Credential_Main_View_H_
