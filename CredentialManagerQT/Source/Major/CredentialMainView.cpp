#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QSplitter>

#include "credential_qt_utils.h"

#include "Major/NavigationView.h"
#include "Major/ContentView.h"

#include "Major/CredentialMainView.h"
#include "Major/CredentialItem.h"
#include "Major/CredentialView.h"
#include "Major/CredentialDialog.h"

#include "Dialog/HintDialog.h"
#include "Dialog/PasswordInput.h"
#include "Dialog/CreateDialog.h"
#include "Dialog/EditDialog.h"

QT_BEGIN_NAMESPACE

CredentialMainView::CredentialMainView(QWidget *parent)
    : QWidget(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint)
{
    ui_utils::SetBackgroundColor(this, ui_utils::g_clrManView);

    _ui.SetupUI(this);

}

bool CredentialMainView::SaveCredential() const
{
    if (m_Credential.GetWord().empty()) return false;
    if (m_strFile.isEmpty()) return false;

    return m_Credential.Save(m_strFile.toStdString().c_str());
}

void CredentialMainView::UpdateCredentail()
{
    /*
    CredentialView * view = new CredentialView(m_Credential, this, this);

    QWidget* old = _ui.m_areaCredential->takeWidget();
    if (nullptr != old) delete old;

    _ui.m_areaCredential->setWidget(view);
    _ui.m_areaCredential->setMaximumSize(view->width() + 20, view->height() + 2);

    if (!_ui.m_btnMotifyName->isEnabled()) _ui.m_btnMotifyName->setEnabled(true);
    if (!_ui.m_btnMotifyWord->isEnabled()) _ui.m_btnMotifyWord->setEnabled(true);

    UpdateSize();
    */
}

void CredentialMainView::UpdateTitle()
{
    // setWindowTitle("Credential - " + QString::fromStdString(m_Credential.GetUser()) + " [" + m_strFile + "]");
}

void CredentialMainView::OnClickedNew()
{
    CreateDialog dlg(this);

    if (QDialog::Accepted == dlg.exec())
    {
        m_Credential.Clear();

        m_Credential.SetUser(dlg.GetUserName().toStdString());
        m_Credential.SetWord(dlg.GetPassword().toStdString());
        m_Credential.UpdateTime();

        m_strFile = dlg.GetFilePath();

        UpdateCredentail();
        UpdateTitle();
        SaveCredential();
    }
}

void CredentialMainView::OnClickedOpen()
{
    /*
    QString strFile = QFileDialog::getOpenFileName(
        this, "Please select a credential file", ".", "credential file(*.credential)");
        */

    QString strFile("def.credential");
    if (strFile.isEmpty()) return;
    
    m_strFile = strFile;

    bnb::memory_type dst;

    switch (bnb::Credential::CheckFile(m_strFile.toUtf8(), &dst))
    {
    case bnb::result_type::rt_file_error:
        HintDialog(hint_type::ht_error, "You selected file error !", this).exec();
        return;
    case bnb::result_type::rt_file_invalid:
        HintDialog(hint_type::ht_error, "You selected file invalid !", this).exec();
        return;
    default:
        break;
    }
    
    PasswordInput dlg(this);
    if (QDialog::Accepted == dlg.exec())
    {
        QString password = /*"123"; //*/ dlg.GetPassword();
        switch (bnb::Credential::Decoding(dst, (const unsigned char*)password.toStdString().c_str(), password.size()))
        {
        case bnb::result_type::rt_password_invalid:
            HintDialog(hint_type::ht_error, "You input password invalid !", this).exec();
            return;
        case bnb::result_type::rt_password_error:
            HintDialog(hint_type::ht_error, "You input password error !", this).exec();
            return;
        case bnb::result_type::rt_file_error:
            HintDialog(hint_type::ht_error, "Anaylze file failed !", this).exec();
            return;
        default:
            break;
        }
    }
    
    if (!m_Credential.FromXml(dst))
    {
        m_Credential.SetWord(dlg.GetPassword().toStdString());
        return;
    }

    UpdateCredentail();
    UpdateTitle();
}

void CredentialMainView::OnClickedMotifyName()
{
    EditUserNameDialog dlg(QString::fromStdString(m_Credential.GetUser()), this, this);

    if (QDialog::Accepted == dlg.exec())
    {
        m_Credential.SetUser(dlg.GetUserName().toStdString());

        UpdateTitle();
        SaveCredential();
    }
}

void CredentialMainView::OnClickedMotifyWord()
{
    EditPasswordDialog dlg(this, this);

    if (QDialog::Accepted == dlg.exec())
    {
        m_Credential.SetWord(dlg.GetPassword().toStdString());
        SaveCredential();
    }
}

bool CredentialMainView::OnAddPlatform()
{
    bnb::platform_type platform;
    EditPlatformDialog dlg(platform, this, this);
    if (QDialog::Accepted == dlg.exec())
    {
        m_Credential.List().Insert(platform);
        UpdateCredentail();
        SaveCredential();
        return true;
    }

    return false;
}

bool CredentialMainView::OnAddAccount(bnb::platform_type* pp)
{
    auto ptr_platform = m_Credential.List().Find(*pp);
    if (ptr_platform)
    {
        bnb::account_type account;
        EditAccountDialog dlg(*pp, account, this, this);
        if (QDialog::Accepted == dlg.exec())
        {
            ptr_platform->m_Value.Insert(account);
            UpdateCredentail();
            SaveCredential();
            return true;
        }
    }

    return false;
}

bool CredentialMainView::OnRemovePlatform(bnb::platform_type * pp)
{
    if (m_Credential.List().Remove(*pp))
    {
        UpdateCredentail();
        SaveCredential();
        return true;
    }
    
    return false;
}

bool CredentialMainView::OnRemoveAccount(bnb::platform_type * pp, bnb::account_type * pa)
{
    auto ptr_platform = m_Credential.List().Find(*pp);
    if (ptr_platform)
    {
        if (ptr_platform->m_Value.Remove(*pa))
        {
            UpdateCredentail();
            SaveCredential();
            return true;
        }
    }

    return false;
}

bool CredentialMainView::OnEditPlatform(bnb::platform_type * pp)
{
    EditPlatformDialog dlg(*pp, this, this);
    if (QDialog::Accepted == dlg.exec())
    {
        SaveCredential();
        return true;
    }

    return false;
}

bool CredentialMainView::OnEditAccount(bnb::platform_type * pp, bnb::account_type * pa)
{
    EditAccountDialog dlg(*pp, *pa, this, this);
    if (QDialog::Accepted == dlg.exec())
    {
        SaveCredential();
        return true;
    }

    return false;
}

bool CredentialMainView::OnViewCredential(bnb::platform_type * pp, bnb::account_type * pa)
{
    /*
    CredentialDialog dlg(m_Credential, this);

    dlg.exec();
    */
    return true;
}

bool CredentialMainView::SetPlatform(bnb::platform_type & p1, const bnb::platform_type & p2)
{
    return m_Credential.List().SetKey(p1, p2);
}

bool CredentialMainView::SetAccount(const bnb::platform_type & pp, bnb::account_type & a1, const bnb::account_type & a2)
{
    auto ptr_platform = m_Credential.List().Find(pp);
    if (ptr_platform)
        return ptr_platform->m_Value.SetKey(a1, a2);

    return false;
}

bool CredentialMainView::ValidateUserName(const bnb::string_type & username)
{
    return !username.empty();
}

bool CredentialMainView::ValidatePassword(const bnb::string_type & password)
{
    return (m_Credential.GetWord() == password);
}

void CredentialMainView::ui_type::SetupUI(CredentialMainView* pView)
{
    pView->setObjectName("CredentialMainView");
    pView->setWindowTitle("Credential Manager");

    QSplitter* phSplitter = new QSplitter(Qt::Horizontal, pView);
    phSplitter->setObjectName("MainSplitter");

    m_treeView = new QTreeWidget(phSplitter);
    m_viewContent = new ContentView(phSplitter);
    
    phSplitter->setHandleWidth(1);
    phSplitter->setOpaqueResize(true);
    phSplitter->setChildrenCollapsible(false);
    phSplitter->setStretchFactor(1, 1);

    m_viewNavigation = new NavigationView(pView);

    QVBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->setMargin(2);
    pMainLayout->setSpacing(2);
    pMainLayout->addWidget(m_viewNavigation);
    pMainLayout->addWidget(phSplitter, 1);

    pView->setLayout(pMainLayout);

    RetranslateUI(pView);
}

void CredentialMainView::ui_type::RetranslateUI(CredentialMainView * pView)
{

}

QT_END_NAMESPACE
