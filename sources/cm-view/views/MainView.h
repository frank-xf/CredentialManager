#pragma once

QT_BEGIN_NAMESPACE

class QDragEnterEvent;
class QDropEvent;
class ToolBar;
class StackView;
class TreeView;

class MainView : public QWidget
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
    void OpenFile(const QString& strFile) { }

private:

    void AddCredential();
    void ClearCredential();
    void CredentialUpdated(unsigned long aType, unsigned long cType);

    /*
    void OnClickedNew() override;
    void OnClickedOpen() override;
    void OnClickedAbout() override;
    void SwitchToView(unsigned int eType, id_type id) override;
    void SwitchToNode(id_type credentialId) override;
    void SwitchToNode(id_type credentialId, id_type platformId) override;
    void SwitchToNode(id_type credentialId, id_type platformId, id_type accountId) override;
    void SwitchToNode(id_type credentialId, id_type platformId, id_type accountId, id_type pairId) override;

    bool OnAddPlatform(id_type credentialId) override;
    bool OnAddAccount(id_type credentialId, id_type platformId) override;
    bool OnAddPair(id_type credentialId, id_type platformId, id_type accountId) override;

    bool OnUpdatePassword(id_type credentialId) override;
    bool OnUpdateCredential(id_type credentialId) override;
    bool OnUpdatePlatform(id_type credentialId, id_type platformId) override;
    bool OnUpdateAccount(id_type credentialId, id_type platformId, id_type accountId) override;
    bool OnUpdatePair(id_type credentialId, id_type platformId, id_type accountId, id_type pairId) override;

    bool OnRemoveCredential(id_type credentialId) override;
    bool OnRemovePlatform(id_type credentialId, id_type platformId) override;
    bool OnRemoveAccount(id_type credentialId, id_type platformId, id_type accountId) override;
    bool OnRemovePair(id_type credentialId, id_type platformId, id_type accountId, id_type pairId) override;

    bool OnMovePlatform(id_type credentialId, id_type platformId, int offset) override;
    bool OnMoveAccount(id_type credentialId, id_type platformId, id_type accountId, int offset) override;
    bool OnMovePair(id_type credentialId, id_type platformId, id_type accountId, id_type pairId, int offset) override;

    bool OnSortPlatform(id_type credentialId, int cln, bool ascending) override;
    bool OnSortAccount(id_type credentialId, id_type platformId, int cln, bool ascending) override;
    bool OnSortPair(id_type credentialId, id_type platformId, id_type accountId, int cln, bool ascending) override;
    */

    void dragEnterEvent(QDragEnterEvent *dee) override;
    void dropEvent(QDropEvent *dee) override;

private:

    ui_type _ui;

    QString m_strFile;

};

QT_END_NAMESPACE
