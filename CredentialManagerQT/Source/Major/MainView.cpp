#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

#ifdef GetUserName
#undef GetUserName
#endif

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QSplitter>

#include "Credential/Credential.h"

#include "credential_qt_string.h"
#include "credential_qt_utils.h"
#include "credential_qt_delegate.h"

#include "Widget/AboutDialog.h"
#include "Widget/HintDialog.h"
#include "Widget/PasswordInput.h"
#include "Widget/NewDialog.h"
#include "Widget/EditDialog.h"

#include "Major/ToolBar.h"
#include "Major/StackView.h"
#include "Major/TreeView.h"
#include "Major/MainView.h"

bnb::Credential& g_Credential()
{
    static bnb::Credential _just_a_credential_object_;
    return (_just_a_credential_object_);
}

QT_BEGIN_NAMESPACE

MainView::MainView(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);

    _ui.SetupUI(this);
}

void MainView::AddCredential()
{
    _ui.m_treeView->AddCredential(g_Credential());

    _ui.m_viewStack->AddCredential(g_Credential());

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
        g_Credential().Save(m_strFile.toStdString().c_str());

        ClearCredential();
        AddCredential();
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

            g_Credential().SetWord(password);

            if (!g_Credential().FromXml(_xml))
            {
                HintDialog(hint_type::ht_error, "Anaylze file failed !", this).exec();
                return;
            }

            m_strFile = strFile;

            ClearCredential();
            AddCredential();
        }
    }
}

void MainView::OnClickedAbout()
{
    AboutDialog().exec();
}

bool MainView::SwitchNode(unsigned int eType, id_type id)
{
    return (_ui.m_viewStack->SwitchToView(static_cast<bnb::credential_enum>(eType), id));
}

bool MainView::OnAddPlatform(id_type credentialId)
{
    if (credentialId == g_Credential().GetID())
    {
        EditPlatformDialog dlg(g_Credential(), nullptr, this);
        if (QDialog::Accepted == dlg.exec())
        {
            g_Credential().Save(m_strFile.toStdString().c_str());

            _ui.m_treeView->AddPlatform(*dlg.GetPlatform());
            _ui.m_viewStack->AddPlatform(*dlg.GetPlatform());
        }

        return true;
    }

    return false;
}

bool MainView::OnAddAccount(id_type credentialId, id_type platformId)
{
    if (credentialId == g_Credential().GetID())
    {
        if (auto ptr_platform = g_Credential().Find(platformId))
        {
            EditAccountDialog dlg(*ptr_platform, nullptr, this);
            if (QDialog::Accepted == dlg.exec())
            {
                g_Credential().Save(m_strFile.toStdString().c_str());

                _ui.m_treeView->AddAccount(*dlg.GetAccount());
                _ui.m_viewStack->AddAccount(*dlg.GetAccount());
            }

            return true;
        }
    }

    return false;
}

bool MainView::OnAddPair(id_type credentialId, id_type platformId, id_type accountId)
{
    if (credentialId == g_Credential().GetID())
    {
        if (auto ptr_account = g_Credential().Find(platformId, accountId))
        {
            EditPairDialog dlg(*ptr_account, nullptr, this);
            if (QDialog::Accepted == dlg.exec())
            {
                g_Credential().Save(m_strFile.toStdString().c_str());

                _ui.m_treeView->AddPair(*dlg.GetPair());
                _ui.m_viewStack->AddPair(*dlg.GetPair());
            }

            return true;
        }
    }

    return false;
}

bool MainView::OnUpdatePassword(id_type credentialId)
{
    if (credentialId == g_Credential().GetID())
    {
        EditPasswordDialog dlg(g_Credential(), this);
        if (QDialog::Accepted == dlg.exec())
        {
            g_Credential().Save(m_strFile.toStdString().c_str());

            _ui.m_viewStack->UpdateCredential(credentialId);
        }

        return true;
    }

    return false;
}

bool MainView::OnUpdateCredential(id_type credentialId)
{
    if (credentialId == g_Credential().GetID())
    {
        EditCredentialDialog dlg(g_Credential(), this);
        if (QDialog::Accepted == dlg.exec())
        {
            g_Credential().Save(m_strFile.toStdString().c_str());

            _ui.m_treeView->UpdateCredential(g_Credential());
            _ui.m_viewStack->UpdateCredential(credentialId);
        }

        return true;
    }

    return false;
}

bool MainView::OnUpdatePlatform(id_type credentialId, id_type platformId)
{
    if (credentialId == g_Credential().GetID())
    {
        if (auto ptr_platform = g_Credential().Find(platformId))
        {
            EditPlatformDialog dlg(g_Credential(), ptr_platform, this);
            if (QDialog::Accepted == dlg.exec())
            {
                g_Credential().Save(m_strFile.toStdString().c_str());

                _ui.m_treeView->UpdatePlatform(*dlg.GetPlatform());
                _ui.m_viewStack->UpdatePlatform(credentialId, platformId);
            }

            return true;
        }
    }

    return false;
}

bool MainView::OnUpdateAccount(id_type credentialId, id_type platformId, id_type accountId)
{
    if (credentialId == g_Credential().GetID())
    {
        if (auto ptr_platform = g_Credential().Find(platformId))
        {
            if (auto ptr_account = ptr_platform->Find(accountId))
            {
                EditAccountDialog dlg(*ptr_platform, ptr_account, this);
                if (QDialog::Accepted == dlg.exec())
                {
                    g_Credential().Save(m_strFile.toStdString().c_str());

                    _ui.m_treeView->UpdateAccount(*dlg.GetAccount());
                    _ui.m_viewStack->UpdateAccount(credentialId, platformId, accountId);
                }
                return true;
            }
        }
    }

    return false;
}

bool MainView::OnUpdatePair(id_type credentialId, id_type platformId, id_type accountId, id_type pairId)
{
    if (credentialId == g_Credential().GetID())
    {
        if (auto ptr_account = g_Credential().Find(platformId, accountId))
        {
            if (auto ptr_pair = ptr_account->Find(pairId))
            {
                EditPairDialog dlg(*ptr_account, ptr_pair, this);
                if (QDialog::Accepted == dlg.exec())
                {
                    g_Credential().Save(m_strFile.toStdString().c_str());

                    _ui.m_treeView->UpdatePair(*dlg.GetPair());
                    _ui.m_viewStack->UpdatePair(credentialId, accountId, pairId);
                }

                return true;
            }
        }
    }

    return false;
}

bool MainView::OnRemoveCredential(id_type credentialId)
{
    if (credentialId == g_Credential().GetID())
    {
        QString strText = "Are you sure to remove credential \"" + To_QString(g_Credential().GetData().GetUser())
            + "\"?\nThis operation is irreversible and the corresponding file will be deleted.";
        if (QDialog::Accepted == ConfirmDialog(strText, this).exec())
        {
            std::vector<id_type> vtrIds{ g_Credential().GetID() };
            g_Credential().PostorderTraversal([&vtrIds](const bnb::platform_node& platform) mutable {
                platform.PostorderTraversal([&vtrIds](const bnb::account_node& account) mutable {
                    account.PostorderTraversal([&vtrIds](const bnb::pair_node& pair) mutable {
                        vtrIds.push_back(pair.GetID());
                    });

                    vtrIds.push_back(account.GetID());
                });

                vtrIds.push_back(platform.GetID());
            });

            g_Credential().Clear();
            QFile::remove(m_strFile);
            m_strFile.clear();

            _ui.m_viewToolBar->UpdatePath(m_strFile);
            _ui.m_treeView->RemoveCredential(credentialId);
            _ui.m_viewStack->RemoveCredential(credentialId, vtrIds);

            return true;
        }
    }

    return false;
}

bool MainView::OnRemovePlatform(id_type credentialId, id_type platformId)
{
    if (credentialId == g_Credential().GetID())
    {
        if (auto ptr_platform = g_Credential().Find(platformId))
        {
            QString strText = To_QString(ptr_platform->GetData().GetName());
            if (QDialog::Accepted == ConfirmDialog("Are you sure to remove platform \"" + strText + "\"?", this).exec())
            {
                std::vector<id_type> vtrIds{ ptr_platform->GetID() };
                ptr_platform->PreorderTraversal([&vtrIds](const bnb::account_node& account) mutable {
                    vtrIds.push_back(account.GetID());
                });

                if (g_Credential().Remove(platformId))
                {
                    g_Credential().Save(m_strFile.toStdString().c_str());

                    _ui.m_treeView->RemovePlatform(credentialId, platformId);
                    _ui.m_viewStack->RemovePlatform(credentialId, platformId, vtrIds);

                    return true;
                }
                else
                {
                    HintDialog(hint_type::ht_error, "Remove the platform \"" + strText + "\" failed !", this).exec();
                }
            }
        }
    }

    return false;
}

bool MainView::OnRemoveAccount(id_type credentialId, id_type platformId, id_type accountId)
{
    if (credentialId == g_Credential().GetID())
    {
        if (auto ptr_platform = g_Credential().Find(platformId))
        {
            if (auto ptr_account = ptr_platform->Find(accountId))
            {
                QString strText = To_QString(ptr_account->GetData().GetName());
                if (QDialog::Accepted == ConfirmDialog("Are you sure to remove account \"" + strText + "\"?", this).exec())
                {
                    std::vector<id_type> vtrIds{ ptr_account->GetID() };
                    ptr_account->PreorderTraversal([&vtrIds](const bnb::pair_node& pair) mutable {
                        vtrIds.push_back(pair.GetID());
                    });

                    if (ptr_platform->Remove(accountId))
                    {
                        g_Credential().Save(m_strFile.toStdString().c_str());

                        _ui.m_treeView->RemoveAccount(credentialId, platformId, accountId);
                        _ui.m_viewStack->RemoveAccount(credentialId, platformId, accountId, vtrIds);

                        return true;
                    }
                    else
                    {
                        HintDialog(hint_type::ht_error, "Remove the account \"" + strText + "\" failed !", this).exec();
                    }
                }
            }
        }
    }

    return false;
}

bool MainView::OnRemovePair(id_type credentialId, id_type platformId, id_type accountId, id_type pairId)
{
    if (credentialId == g_Credential().GetID())
    {
        if (auto ptr_account = g_Credential().Find(platformId, accountId))
        {
            if (auto ptr_pair = ptr_account->Find(pairId))
            {
                QString strText = To_QString(ptr_pair->GetData().GetKey());
                if (QDialog::Accepted == ConfirmDialog("Are you sure to remove pair \"" + strText + "\"?", this).exec())
                {
                    if (ptr_account->Remove(pairId))
                    {
                        g_Credential().Save(m_strFile.toStdString().c_str());

                        _ui.m_treeView->RemovePair(credentialId, platformId, accountId, pairId);
                        _ui.m_viewStack->RemovePair(credentialId, platformId, accountId, pairId);

                        return true;
                    }
                    else
                    {
                        HintDialog(hint_type::ht_error, "Remove the pair \"" + strText + "\" failed !", this).exec();
                    }
                }
            }
        }
    }

    return false;
}

bool MainView::OnMovePlatform(id_type credentialId, id_type platformId, int offset)
{
    if (credentialId == g_Credential().GetID())
    {
        if (g_Credential().Move(platformId, offset))
        {
            g_Credential().Save(m_strFile.toStdString().c_str());

            _ui.m_treeView->MovePlatform(credentialId, platformId, offset);
            _ui.m_viewStack->UpdateTable(credentialId);

            return true;
        }
    }

    return false;
}

bool MainView::OnMoveAccount(id_type credentialId, id_type platformId, id_type accountId, int offset)
{
    if (credentialId == g_Credential().GetID())
    {
        if (auto ptr_platform = g_Credential().Find(platformId))
        {
            if (ptr_platform->Move(accountId, offset))
            {
                g_Credential().Save(m_strFile.toStdString().c_str());

                _ui.m_treeView->MoveAccount(credentialId, platformId, accountId, offset);
                _ui.m_viewStack->UpdateTable(credentialId, platformId);

                return true;
            }
        }
    }

    return false;
}

bool MainView::OnMovePair(id_type credentialId, id_type platformId, id_type accountId, id_type pairId, int offset)
{
    if (credentialId == g_Credential().GetID())
    {
        if (auto ptr_account = g_Credential().Find(platformId, accountId))
        {
            if (ptr_account->Move(pairId, offset))
            {
                g_Credential().Save(m_strFile.toStdString().c_str());

                _ui.m_treeView->MovePair(credentialId, platformId, accountId, pairId, offset);
                _ui.m_viewStack->UpdateTable(credentialId, platformId, accountId);

                return true;
            }
        }
    }

    return false;
}

bool MainView::OnSortPlatform(id_type credentialId, int cln, bool ascending)
{
    if (credentialId == g_Credential().GetID())
    {
        switch (cln)
        {
        case 0:
            g_Credential().Sort([&ascending](const bnb::platform_type& a, const bnb::platform_type& b) {
                if (a.GetName() == b.GetName())
                    return (ascending ? (a.GetUrl() < b.GetUrl()) : (b.GetUrl() < a.GetUrl()));
                else
                    return (ascending ? (a.GetName() < b.GetName()) : (b.GetName() < a.GetName()));
            });
            break;
        case 1:
            g_Credential().Sort([&ascending](const bnb::platform_type& a, const bnb::platform_type& b) {
                if (a.GetUrl() == b.GetUrl())
                    return (ascending ? (a.GetName() < b.GetName()) : (b.GetName() < a.GetName()));
                else
                    return (ascending ? (a.GetUrl() < b.GetUrl()) : (b.GetUrl() < a.GetUrl()));
            });
            break;
        default:
            return false;
        }

        g_Credential().Save(m_strFile.toStdString().c_str());

        _ui.m_treeView->Reschedule(g_Credential());
        _ui.m_viewStack->UpdateTable(credentialId);

        return true;
    }

    return false;
}

bool MainView::OnSortAccount(id_type credentialId, id_type platformId, int cln, bool ascending)
{
    if (credentialId == g_Credential().GetID())
    {
        if (auto ptr_platform = g_Credential().Find(platformId))
        {
            switch (cln)
            {
            case 0:
                ptr_platform->Sort([&ascending](const bnb::account_type& a, const bnb::account_type& b) {
                    return (ascending ? (a.GetName() < b.GetName()) : (b.GetName() < a.GetName()));
                });
                break;
            default:
                return false;
            }

            g_Credential().Save(m_strFile.toStdString().c_str());

            _ui.m_treeView->Reschedule(*ptr_platform);
            _ui.m_viewStack->UpdateTable(credentialId, platformId);

            return true;
        }
    }

    return false;
}

bool MainView::OnSortPair(id_type credentialId, id_type platformId, id_type accountId, int cln, bool ascending)
{
    if (credentialId == g_Credential().GetID())
    {
        if (auto ptr_account = g_Credential().Find(platformId, accountId))
        {
            switch (cln)
            {
            case 0:
                ptr_account->Sort([&ascending](const bnb::pair_type& a, const bnb::pair_type& b) {
                    if (a.GetKey() == b.GetKey())
                        return (ascending ? (a.GetValue() < b.GetValue()) : (b.GetValue() < a.GetValue()));
                    else
                        return (ascending ? (a.GetKey() < b.GetKey()) : (b.GetKey() < a.GetKey()));
                });
                break;
            case 1:
                ptr_account->Sort([&ascending](const bnb::pair_type& a, const bnb::pair_type& b) {
                    if (a.GetValue() == b.GetValue())
                        return (ascending ? (a.GetKey() < b.GetKey()) : (b.GetKey() < a.GetKey()));
                    else
                        return (ascending ? (a.GetValue() < b.GetValue()) : (b.GetValue() < a.GetValue()));
                });
                break;
            default:
                return false;
            }

            g_Credential().Save(m_strFile.toStdString().c_str());

            _ui.m_treeView->Reschedule(*ptr_account);
            _ui.m_viewStack->UpdateTable(credentialId, platformId, accountId);

            return true;
        }
    }

    return false;
}

bool MainView::nativeEvent(const QByteArray &eventType, void *pMessage, long *pResult)
{
    if (MSG* pMsg = static_cast<MSG*>(pMessage))
    {
        switch (pMsg->message)
        {
        case WM_ACTIVATE:
        {
            switch (pMsg->wParam)
            {
            case WA_INACTIVE:
            {
                if (g_Credential().GetData().IsValid())
                    setWindowOpacity(0.16);
                break;
            }
            case WA_ACTIVE:
            case WA_CLICKACTIVE:
                setWindowOpacity(0.96);
                break;
            default:
                break;
            }
            break;
        }
        default:
            break;
        }
    }

    return QWidget::nativeEvent(eventType, pMessage, pResult);
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

    m_treeView = new TreeView(pView, phSplitter);
    m_viewStack = new StackView(pView, phSplitter);
    m_viewToolBar = new ToolBar(pView, pView);

    QVBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->setMargin(0);
    pMainLayout->setSpacing(1);
    pMainLayout->addWidget(m_viewToolBar);
    pMainLayout->addWidget(phSplitter, 1);

    pView->setLayout(pMainLayout);

    RetranslateUI(pView);
}

void MainView::ui_type::RetranslateUI(MainView * pView) { }

QT_END_NAMESPACE

bnb::string_type From_QString(const QT_PREPEND_NAMESPACE(QString)& str)
{
    return str.toStdWString();
}

QT_PREPEND_NAMESPACE(QString) To_QString(const bnb::string_type& str)
{
    return QT_PREPEND_NAMESPACE(QString)::fromStdWString(str);
}

void Init()
{
    QT_PREPEND_NAMESPACE(MainView)* _viewMain = new QT_PREPEND_NAMESPACE(MainView);
    _viewMain->show();
}
