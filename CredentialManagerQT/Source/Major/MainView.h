#ifndef _bnb_Credential_Main_View_H_
#define _bnb_Credential_Main_View_H_

QT_BEGIN_NAMESPACE

class ToolBar;
class StackView;
class TreeView;

class MainView : public QWidget, public DelegateMainView
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

    void AddCredential();
    void ClearCredential();

    void OnClickedNew() override;
    void OnClickedOpen() override;
    void OnClickedAbout() override;
    bool SwitchNode(unsigned int eType, unsigned int id) override;

    bool OnAddPlatform(unsigned int id1) override;
    bool OnAddAccount(unsigned int id1, unsigned int id2) override;
    bool OnAddPair(unsigned int id1, unsigned int id2, unsigned int id3) override;

    bool OnUpdatePassword(unsigned int id1) override;
    bool OnUpdateCredential(unsigned int id1) override;
    bool OnUpdatePlatform(unsigned int id1, unsigned int id2) override;
    bool OnUpdateAccount(unsigned int id1, unsigned int id2, unsigned int id3) override;
    bool OnUpdatePair(unsigned int id1, unsigned int id2, unsigned int id3, unsigned int id4) override;

    bool OnRemovePlatform(unsigned int id1, unsigned int id2) override;
    bool OnRemoveAccount(unsigned int id1, unsigned int id2, unsigned int id3) override;
    bool OnRemovePair(unsigned int id1, unsigned int id2, unsigned int id3, unsigned int id4) override;

private:

    ui_type _ui;

    QString m_strFile;

};

QT_END_NAMESPACE

#endif // _bnb_Credential_Main_View_H_
