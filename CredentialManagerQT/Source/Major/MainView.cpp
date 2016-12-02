#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QMenu>
#include <QtWidgets/QSplitter>

#include<iostream>

#include "Credential/Credential.h"

#include "credential_qt_string.h"
#include "credential_qt_utils.h"

#include "Widget/AboutDialog.h"
#include "Widget/HintDialog.h"
#include "Widget/PasswordInput.h"
#include "Widget/NewDialog.h"
#include "Widget/EditDialog.h"

#include "Major/ToolBar.h"
#include "Major/StackView.h"
#include "Major/TreeView.h"
#include "Major/MainView.h"

void UTF8toANSI(std::string &strUTF8);

bnb::Credential& g_Credential()
{
    static bnb::Credential _just_a_credential_;
    return (_just_a_credential_);
}

QT_BEGIN_NAMESPACE

inline static bnb::credential_type GetItemType(const QTreeWidgetItem& item)
{
    return static_cast<bnb::credential_type>(item.data(0, Qt::UserRole).toUInt());
}

MainView::MainView(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowOpacity(0.96);

    _ui.SetupUI(this);

    QObject::connect(_ui.m_viewToolBar->UI().m_btnNew, &QPushButton::clicked, this, &MainView::OnClickedNew);
    QObject::connect(_ui.m_viewToolBar->UI().m_btnOpen, &QPushButton::clicked, this, &MainView::OnClickedOpen);
    QObject::connect(_ui.m_viewToolBar->UI().m_btnAbout, &QPushButton::clicked, this, &MainView::OnClickedAbout);

    QObject::connect(_ui.m_treeView, &QTreeWidget::customContextMenuRequested, this, &MainView::OnTreeContextMenu);
    QObject::connect(_ui.m_treeView, &QTreeWidget::currentItemChanged, this, &MainView::OnItemChanged);
    QObject::connect(_ui.m_treeView, &QTreeWidget::itemDoubleClicked, this, &MainView::OnDoubleClickedItem);

    QObject::connect(_ui.m_actAddAccount, &QAction::triggered, this, &MainView::OnAddAccount);
    QObject::connect(_ui.m_actAddPlatform, &QAction::triggered, this, &MainView::OnAddPlatform);
    QObject::connect(_ui.m_actAddProperty, &QAction::triggered, this, &MainView::OnAddProperty);
    QObject::connect(_ui.m_actDelAccount, &QAction::triggered, this, &MainView::OnRemoveAccount);
    QObject::connect(_ui.m_actDelPlatform, &QAction::triggered, this, &MainView::OnRemovePlatform);
    QObject::connect(_ui.m_actDelProperty, &QAction::triggered, this, &MainView::OnRemoveProperty);
    QObject::connect(_ui.m_actEditAccount, &QAction::triggered, this, &MainView::OnEditAccount);
    QObject::connect(_ui.m_actEditPlatform, &QAction::triggered, this, &MainView::OnEditPlatform);
    QObject::connect(_ui.m_actEditProperty, &QAction::triggered, this, &MainView::OnEditProperty);
    QObject::connect(_ui.m_actModifyPassword, &QAction::triggered, this, &MainView::OnMotifyPassword);
    QObject::connect(_ui.m_actEditCredential, &QAction::triggered, this, &MainView::OnEditCredential);
}

bool MainView::SaveCredential()
{
    g_Credential().UpdateTime();

    bnb::memory_type xml;

    g_Credential().ToXml(xml);
    std::string _xml((char*)xml.c_str(), xml.size());
    UTF8toANSI(_xml);

    printf(_xml.c_str());

    return g_Credential().Save(m_strFile.toStdString().c_str());
}

void MainView::InitCredential()
{
    auto item_root = _ui.m_treeView->AddRoot(g_Credential());

    auto view_credential = _ui.m_viewStack->AddCredential(g_Credential());

    g_Credential().List().PreorderTraversal([this, item_root](const bnb::platform_list::data_type& platform) {
        QTreeWidgetItem* item_platform = _ui.m_treeView->AddPlatform(item_root, platform);

        _ui.m_viewStack->AddPlatform(platform);

        platform.m_Value.PreorderTraversal([this, item_platform](const bnb::account_list::data_type& account) {
            QTreeWidgetItem* item_account = _ui.m_treeView->AddAccount(item_platform, account);

            _ui.m_viewStack->AddAccount(account);

            account.m_Value.PreorderTraversal([this, item_account](const bnb::property_list::data_type& property) {
                QTreeWidgetItem* item_property = _ui.m_treeView->AddProperty(item_account, property);

                _ui.m_viewStack->AddProperty(property);
            });

            item_account->setExpanded(false);
        });
    });

    _ui.m_viewStack->SwitchToCredential(g_Credential().GetID());

    _ui.m_viewToolBar->UpdatePath(m_strFile);
}

void MainView::ClearCredential()
{
    _ui.m_treeView->ClearCredential();
    _ui.m_viewStack->ClearCredential();
}

void MainView::OnClickedNew()
{
    NewDialog dlg(this);

    if (QDialog::Accepted == dlg.exec())
    {
        m_strFile = dlg.GetFileName();

        g_Credential().Clear();
        g_Credential().SetWord(From_QString(dlg.GetPassword()));
        g_Credential().SetUser(From_QString(dlg.GetUserName()));
        g_Credential().SetComment(From_QString(dlg.GetComment()));
        
        SaveCredential();
        ClearCredential();
        InitCredential();
    }
}

void MainView::OnClickedOpen()
{
    QString strFile = QFileDialog::getOpenFileName(
        this, "Please select a credential file", ".", "credential file(*.credential)");

    if (!strFile.isEmpty())
    {
        bnb::memory_type _xml;

        if (!bnb::Credential::CheckFile(strFile.toStdString().c_str(), &_xml))
        {
            HintDialog(hint_type::ht_error, "You selected file invalid !", this).exec();
            return;
        }

        PasswordInput dlg(this);
        if (QDialog::Accepted == dlg.exec())
        {
            bnb::string_type password = From_QString(dlg.GetPassword());
            if (password.empty())
            {
                HintDialog(hint_type::ht_error, "Please input a password !", this).exec();
                return;
            }

            if (!bnb::Credential::Decoding(_xml, (const unsigned char*)password.c_str(), password.size() * sizeof(bnb::char_type)))
            {
                HintDialog(hint_type::ht_error, "You input password error !", this).exec();
                return;
            }

            if (!g_Credential().FromXml(_xml))
            {
                HintDialog(hint_type::ht_error, "Anaylze file failed !", this).exec();
                return;
            }

            m_strFile = strFile;

            g_Credential().SetWord(password);

            ClearCredential();
            InitCredential();
        }
    }
}

void MainView::OnClickedAbout()
{
    AboutDialog().exec();
}

void MainView::OnTreeContextMenu(const QPoint & pos)
{
    QTreeWidgetItem* pItem = _ui.m_treeView->itemAt(pos);

    QMenu treeMenu(_ui.m_treeView);
    treeMenu.setStyleSheet("QMenu{ border: 1px solid gray; background-color: white; }"
        "QMenu::item{ padding: 8px 20px 8px 20px; }"
        "QMenu::item:selected{ background:#C0F0C0;}"
        "QMenu::separator{ height: 1px; background: gray; margin-left: 2px; margin-right: 2px;}");

    if (pItem)
    {
        switch (static_cast<bnb::credential_type>(pItem->data(0, Qt::UserRole).toUInt()))
        {
        case bnb::credential_type::ct_credential:
            treeMenu.addAction(_ui.m_actEditCredential);
            treeMenu.addAction(_ui.m_actModifyPassword);
            treeMenu.addSeparator();
            treeMenu.addAction(_ui.m_actAddPlatform);
            break;
        case bnb::credential_type::ct_property:
            treeMenu.addAction(_ui.m_actEditProperty);
            treeMenu.addAction(_ui.m_actDelProperty);
            break;
        case bnb::credential_type::ct_account:
            treeMenu.addAction(_ui.m_actEditAccount);
            treeMenu.addAction(_ui.m_actDelAccount);
            treeMenu.addSeparator();
            treeMenu.addAction(_ui.m_actAddProperty);
            break;
        case bnb::credential_type::ct_platform:
            treeMenu.addAction(_ui.m_actEditPlatform);
            treeMenu.addAction(_ui.m_actDelPlatform);
            treeMenu.addSeparator();
            treeMenu.addAction(_ui.m_actAddAccount);
            break;
        default:
            break;
        }
    }

    treeMenu.exec(QCursor::pos());
}

void MainView::OnItemChanged(QTreeWidgetItem * cur, QTreeWidgetItem * pre)
{
    if (cur)
    {
        switch (GetItemType(*cur))
        {
        case bnb::credential_type::ct_credential:
        {
            if (g_Credential().IsValid())
                _ui.m_viewStack->SwitchToCredential(g_Credential().GetID());
            else
                _ui.m_viewStack->SwitchToHint("You haven\'t opened any credential !");

            return;
        }
        case bnb::credential_type::ct_platform:
        {
            if (auto ptr_platform = g_Credential().List().Find({ From_QString(cur->text(0)) }))
                _ui.m_viewStack->SwitchToPlatform(ptr_platform->m_Key.GetID());

            return;
        }
        case bnb::credential_type::ct_property:
        {
            auto item_account = cur->parent();
            if (item_account && bnb::credential_type::ct_account == GetItemType(*item_account))
                cur = item_account;
            else
                return;
        }
        case bnb::credential_type::ct_account:
        {
            auto item_platform = cur->parent();
            if (item_platform && bnb::credential_type::ct_platform == GetItemType(*item_platform))
                if (auto ptr_platform = g_Credential().List().Find({ From_QString(item_platform->text(0)) }))
                    if (auto ptr_account = ptr_platform->m_Value.Find({ From_QString(cur->text(0)) }))
                        _ui.m_viewStack->SwitchToAccount(ptr_account->m_Key.GetID());

            return;
        }
        default:
            break;
        }
    }
}

void MainView::OnDoubleClickedItem(QTreeWidgetItem * pItem, int index)
{
    std::cout << "index: " << index << ", text: " << pItem->text(0).toStdString() << std::endl;

    if (0 == index && pItem)
    {
        switch (GetItemType(*pItem))
        {
        case bnb::credential_type::ct_credential:
        {
            if (EditCredential(pItem))
                return;

            break;
        }
        case bnb::credential_type::ct_platform:
        {
            if (EditPlatform(pItem))
                return;

            break;
        }
        case bnb::credential_type::ct_property:
        {
            if (EditProperty(pItem))
                return;

            break;
        }
        case bnb::credential_type::ct_account:
        {
            if (EditAccount(pItem))
                return;

            break;
        }
        default:
            break;
        }
    }

    HintDialog(hint_type::ht_warning, "The parameter error !", this).exec();
}

void MainView::OnAddPlatform()
{
    if (QTreeWidgetItem* item_credential = _ui.m_treeView->currentItem())
        if (bnb::credential_type::ct_credential == GetItemType(*item_credential))
            if (AddPlatform(item_credential))
                return;

    HintDialog(hint_type::ht_warning, "The parameter error !", this).exec();
}

void MainView::OnAddAccount()
{
    if (QTreeWidgetItem* item_platform = _ui.m_treeView->currentItem())
        if (bnb::credential_type::ct_platform == GetItemType(*item_platform))
            if (AddAccount(item_platform))
                return;

    HintDialog(hint_type::ht_warning, "The parameter error !", this).exec();
}

void MainView::OnAddProperty()
{
    if (QTreeWidgetItem* item_account = _ui.m_treeView->currentItem())
        if (bnb::credential_type::ct_account == GetItemType(*item_account))
            if (AddProperty(item_account))
                return;

    HintDialog(hint_type::ht_warning, "The parameter error !", this).exec();
}

void MainView::OnMotifyPassword()
{
    EditPasswordDialog dlg(g_Credential(), this);

    if (QDialog::Accepted == dlg.exec())
    {
        SaveCredential();
    }
}

void MainView::OnEditCredential()
{
    if (QTreeWidgetItem* item_credential = _ui.m_treeView->currentItem())
        if (bnb::credential_type::ct_credential == GetItemType(*item_credential))
            if (EditCredential(item_credential))
                return;

    HintDialog(hint_type::ht_warning, "The parameter error !", this).exec();
}

void MainView::OnEditPlatform()
{
    if (QTreeWidgetItem* item_platform = _ui.m_treeView->currentItem())
        if (bnb::credential_type::ct_platform == GetItemType(*item_platform))
            if (EditPlatform(item_platform))
                return;

    HintDialog(hint_type::ht_warning, "The parameter error !", this).exec();
}

void MainView::OnEditAccount()
{
    if (QTreeWidgetItem* item_account = _ui.m_treeView->currentItem())
        if (bnb::credential_type::ct_account == GetItemType(*item_account))
            if (EditAccount(item_account))
                return;

    HintDialog(hint_type::ht_warning, "The parameter error !", this).exec();
}

void MainView::OnEditProperty()
{
    if (QTreeWidgetItem* item_property = _ui.m_treeView->currentItem())
        if (bnb::credential_type::ct_property == GetItemType(*item_property))
            if (EditProperty(item_property))
                return;

    HintDialog(hint_type::ht_warning, "The parameter error !", this).exec();
}

void MainView::OnRemovePlatform()
{
    if (QTreeWidgetItem* item_platform = _ui.m_treeView->currentItem())
    {
        if (bnb::credential_type::ct_platform == GetItemType(*item_platform))
        {
            QString strText = "Are you sure to remove the platform \"" + item_platform->text(0) + "\"?";
            if (QDialog::Accepted == ConfirmDialog(strText, this).exec())
                if (!RemovePlatform(item_platform))
                    HintDialog(hint_type::ht_error, "Remove platform failed !", this).exec();

            return;
        }
    }

    HintDialog(hint_type::ht_warning, "The platform\'s parameter error !", this).exec();
}

void MainView::OnRemoveAccount()
{
    if (QTreeWidgetItem* item_account = _ui.m_treeView->currentItem())
    {
        if (bnb::credential_type::ct_account == GetItemType(*item_account))
        {
            QString strText = "Are you sure to remove the account \"" + item_account->text(0) + "\"?";
            if (QDialog::Accepted == ConfirmDialog(strText, this).exec())
                if (!RemoveAccount(item_account))
                    HintDialog(hint_type::ht_error, "Remove account failed !", this).exec();

            return;
        }
    }

    HintDialog(hint_type::ht_warning, "The account\'s parameter error !", this).exec();
}

void MainView::OnRemoveProperty()
{
    if (QTreeWidgetItem* item_property = _ui.m_treeView->currentItem())
    {
        if (bnb::credential_type::ct_property == GetItemType(*item_property))
        {
            QString strText = "Are you sure to remove the key \"" + item_property->text(0) + "\"?";
            if (QDialog::Accepted == ConfirmDialog(strText, this).exec())
                if (!RemoveProperty(item_property))
                    HintDialog(hint_type::ht_error, "Remove property failed !", this).exec();

            return;
        }
    }

    HintDialog(hint_type::ht_warning, "The property\'s parameter error !", this).exec();
}

bool MainView::AddPlatform(QTreeWidgetItem* item_credential)
{
    EditPlatformDialog dlg(g_Credential(), nullptr, this);
    if (QDialog::Accepted == dlg.exec())
    {
        SaveCredential();

        _ui.m_treeView->AddPlatform(item_credential, *dlg.GetPlatform());
        _ui.m_viewStack->AddPlatform(*dlg.GetPlatform(), g_Credential().GetID());
        _ui.m_viewStack->UpdateCredential(g_Credential().GetID());
    }

    return true;
}

bool MainView::AddAccount(QTreeWidgetItem* item_platform)
{
    auto ptr_platform = g_Credential().List().Find({ From_QString(item_platform->text(0)) });
    if (ptr_platform)
    {
        EditAccountDialog dlg(*ptr_platform, nullptr, this);
        if (QDialog::Accepted == dlg.exec())
        {
            SaveCredential();

            _ui.m_treeView->AddAccount(item_platform, *dlg.GetAccount());
            _ui.m_viewStack->AddAccount(*dlg.GetAccount(), ptr_platform->m_Key.GetID());
            _ui.m_viewStack->UpdateCredential(g_Credential().GetID());
        }

        return true;
    }

    return false;
}

bool MainView::AddProperty(QTreeWidgetItem * item_account)
{
    QTreeWidgetItem* item_platform = item_account->parent();
    if (item_platform && bnb::credential_type::ct_platform == GetItemType(*item_platform))
    {
        if (auto ptr_platform = g_Credential().List().Find({ From_QString(item_platform->text(0)) }))
        {
            if (auto ptr_account = ptr_platform->m_Value.Find({ From_QString(item_account->text(0)) }))
            {
                EditPropertyDialog dlg(*ptr_account, nullptr, this);
                if (QDialog::Accepted == dlg.exec())
                {
                    SaveCredential();

                    _ui.m_treeView->AddProperty(item_account, *dlg.GetProperty());
                    _ui.m_viewStack->AddProperty(*dlg.GetProperty(), ptr_account->m_Key.GetID());
                    _ui.m_viewStack->UpdateCredential(g_Credential().GetID());
                }

                return true;
            }
        }
    }

    return false;
}

bool MainView::EditCredential(QTreeWidgetItem * item_credential)
{
    EditCredentialDialog dlg(g_Credential(), this);

    if (QDialog::Accepted == dlg.exec())
    {
        SaveCredential();

        _ui.m_treeView->UpdateHeader(To_QString(g_Credential().GetUser()));
        _ui.m_viewStack->UpdateCredential(g_Credential().GetID());
    }

    return true;
}

bool MainView::EditPlatform(QTreeWidgetItem * item_platform)
{
    auto ptr_platform = g_Credential().List().Find({ From_QString(item_platform->text(0)) });
    if (ptr_platform)
    {
        EditPlatformDialog dlg(g_Credential(), ptr_platform, this);
        if (QDialog::Accepted == dlg.exec())
        {
            SaveCredential();

            item_platform->setText(0, To_QString(ptr_platform->m_Key.m_strName));
            _ui.m_viewStack->UpdatePlatform(g_Credential().GetID(), ptr_platform->m_Key.GetID());
            _ui.m_viewStack->UpdateCredential(g_Credential().GetID());
        }

        return true;
    }

    return false;
}

bool MainView::EditAccount(QTreeWidgetItem * item_account)
{
    QTreeWidgetItem* item_platform = item_account->parent();
    if (item_platform && bnb::credential_type::ct_platform == GetItemType(*item_platform))
    {
        if (auto ptr_platform = g_Credential().List().Find({ From_QString(item_platform->text(0)) }))
        {
            if (auto ptr_account = ptr_platform->m_Value.Find({ From_QString(item_account->text(0)) }))
            {
                EditAccountDialog dlg(*ptr_platform, ptr_account, this);
                if (QDialog::Accepted == dlg.exec())
                {
                    SaveCredential();

                    item_account->setText(0, To_QString(ptr_account->m_Key.m_strName));
                    _ui.m_viewStack->UpdateAccount(ptr_platform->m_Key.GetID(), ptr_account->m_Key.GetID());
                    _ui.m_viewStack->UpdateCredential(g_Credential().GetID());
                }

                return true;
            }
        }
    }

    return false;
}

bool MainView::EditProperty(QTreeWidgetItem * item_property)
{
    QTreeWidgetItem* item_account = item_property->parent();
    if (item_account && bnb::credential_type::ct_account == GetItemType(*item_account))
    {
        QTreeWidgetItem* item_platform = item_account->parent();
        if (item_platform && bnb::credential_type::ct_platform == GetItemType(*item_platform))
        {
            if (auto ptr_platform = g_Credential().List().Find({ From_QString(item_platform->text(0)) }))
            {
                if (auto ptr_account = ptr_platform->m_Value.Find({ From_QString(item_account->text(0)) }))
                {
                    if (auto ptr_property = ptr_account->m_Value.Find({ From_QString(item_property->text(0)) }))
                    {
                        EditPropertyDialog dlg(*ptr_account, ptr_property, this);
                        if (QDialog::Accepted == dlg.exec())
                        {
                            SaveCredential();

                            item_property->setText(0, To_QString(ptr_property->m_Key.m_strName));
                            _ui.m_viewStack->UpdateProperty(ptr_account->m_Key.GetID(), ptr_property->m_Key.GetID());
                            _ui.m_viewStack->UpdateCredential(g_Credential().GetID());
                        }

                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool MainView::RemovePlatform(QTreeWidgetItem * item_platform)
{
    auto ptr_platform = g_Credential().List().Find({ From_QString(item_platform->text(0)) });
    if (ptr_platform)
    {
        std::vector<unsigned int> vtrIds{ ptr_platform->m_Key.GetID() };
        ptr_platform->m_Value.PreorderTraversal([&vtrIds](const bnb::account_list::data_type& account) mutable {
            vtrIds.push_back(account.m_Key.GetID());
        });

        if (g_Credential().List().Remove(ptr_platform->m_Key))
        {
            SaveCredential();

            auto ptr_parent = item_platform->parent();
            if (ptr_parent)
                ptr_parent->removeChild(item_platform);
            else
                _ui.m_treeView->takeTopLevelItem(_ui.m_treeView->indexOfTopLevelItem(item_platform));

            delete item_platform;

            _ui.m_viewStack->RemovePlatform(g_Credential().GetID(), vtrIds);
            _ui.m_viewStack->UpdateCredential(g_Credential().GetID());

            return true;
        }
    }

    return false;
}

bool MainView::RemoveAccount(QTreeWidgetItem * item_account)
{
    QTreeWidgetItem* item_platform = item_account->parent();
    if (item_platform && bnb::credential_type::ct_platform == GetItemType(*item_platform))
    {
        if (auto ptr_platform = g_Credential().List().Find({ From_QString(item_platform->text(0)) }))
        {
            if (auto ptr_account = ptr_platform->m_Value.Find({ From_QString(item_account->text(0)) }))
            {
                std::vector<unsigned int> vtrIds{ ptr_account->m_Key.GetID() };
                if (ptr_platform->m_Value.Remove(ptr_account->m_Key))
                {
                    SaveCredential();

                    item_platform->removeChild(item_account);
                    delete item_account;

                    _ui.m_viewStack->RemoveAccount(ptr_platform->m_Key.GetID(), vtrIds);
                    _ui.m_viewStack->UpdateCredential(g_Credential().GetID());

                    return true;
                }
            }
        }
    }

    return false;
}

bool MainView::RemoveProperty(QTreeWidgetItem* item_property)
{
    QTreeWidgetItem* item_account = item_property->parent();
    if (item_account && bnb::credential_type::ct_account == GetItemType(*item_account))
    {
        QTreeWidgetItem* item_platform = item_account->parent();
        if (item_platform && bnb::credential_type::ct_platform == GetItemType(*item_platform))
        {
            if (auto ptr_platform = g_Credential().List().Find({ From_QString(item_platform->text(0)) }))
            {
                if (auto ptr_account = ptr_platform->m_Value.Find({ From_QString(item_account->text(0)) }))
                {
                    if (auto ptr_property = ptr_account->m_Value.Find({ From_QString(item_property->text(0)) }))
                    {
                        std::vector<unsigned int> vtrIds{ ptr_property->m_Key.GetID() };
                        if (ptr_account->m_Value.Remove(ptr_property->m_Key))
                        {
                            SaveCredential();

                            item_account->removeChild(item_property);
                            delete item_property;

                            _ui.m_viewStack->RemoveProperty(ptr_account->m_Key.GetID(), vtrIds);
                            _ui.m_viewStack->UpdateCredential(g_Credential().GetID());

                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

void MainView::ui_type::SetupUI(MainView* pView)
{
    pView->setObjectName("MainView");
    pView->setWindowTitle("Credential Manager");
    ui_utils::SetBackgroundColor(pView, ui_utils::g_clrManView);

    QSplitter* phSplitter = new QSplitter(Qt::Horizontal, pView);
    phSplitter->setObjectName("MainSplitter");
    phSplitter->setHandleWidth(1);
    phSplitter->setOpaqueResize(true);
    phSplitter->setChildrenCollapsible(false);

    m_treeView = new TreeView(phSplitter);

    m_viewStack = new StackView(phSplitter);

    m_viewToolBar = new ToolBar(pView);

    QVBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->setMargin(0);
    pMainLayout->setSpacing(1);
    pMainLayout->addWidget(m_viewToolBar);
    pMainLayout->addWidget(phSplitter, 1);

    pView->setLayout(pMainLayout);

    m_actAddAccount = new QAction(pView);
    m_actAddPlatform = new QAction(pView);
    m_actAddProperty = new QAction(pView);
    m_actDelAccount = new QAction(pView);
    m_actDelPlatform = new QAction(pView);
    m_actDelProperty = new QAction(pView);
    m_actEditAccount = new QAction(pView);
    m_actEditPlatform = new QAction(pView);
    m_actEditProperty = new QAction(pView);
    m_actModifyPassword = new QAction(pView);
    m_actEditCredential = new QAction(pView);

    RetranslateUI(pView);
}

void MainView::ui_type::RetranslateUI(MainView * pView)
{
    m_actAddAccount->setText("PushBack Account");
    m_actAddPlatform->setText("PushBack Platform");
    m_actAddProperty->setText("PushBack Property");
    m_actDelAccount->setText("Remove Account");
    m_actDelPlatform->setText("Remove Platform");
    m_actDelProperty->setText("Remove Property");
    m_actEditAccount->setText("Edit Account");
    m_actEditPlatform->setText("Edit Platform");
    m_actEditProperty->setText("Edit Property");
    m_actModifyPassword->setText("Modify Password");
    m_actEditCredential->setText("Edit Credential");
}

void Init()
{
    MainView* _viewMain = new MainView;
    _viewMain->show();
}

QT_END_NAMESPACE

bnb::string_type From_QString(const QT_PREPEND_NAMESPACE(QString)& str)
{
    return str.toStdWString();
}

QT_PREPEND_NAMESPACE(QString) To_QString(const bnb::string_type& str)
{
    return QT_PREPEND_NAMESPACE(QString)::fromStdWString(str);
}

#include <windows.h>

void UTF8toANSI(std::string &strUTF8)
{
    //获取转换为多字节后需要的缓冲区大小，创建多字节缓冲区  
    unsigned int nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8.c_str(), -1, NULL, NULL);
    wchar_t *wszBuffer = new wchar_t[nLen + 1];
    nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8.c_str(), -1, wszBuffer, nLen);
    wszBuffer[nLen] = 0;

    nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
    char *szBuffer = new char[nLen + 1];
    nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
    szBuffer[nLen] = 0;

    strUTF8 = szBuffer;
    //清理内存  
    delete[]szBuffer;
    delete[]wszBuffer;
}
