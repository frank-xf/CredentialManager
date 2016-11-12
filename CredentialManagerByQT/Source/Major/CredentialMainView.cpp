#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QLabel>

#include "credential_qt_utils.h"

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

    UpdateSize();

    QObject::connect(_ui.m_btnNew, &QPushButton::clicked, this, &CredentialMainView::OnClickedNew);
    QObject::connect(_ui.m_btnOpen, &QPushButton::clicked, this, &CredentialMainView::OnClickedOpen);
    QObject::connect(_ui.m_btnMotifyName, &QPushButton::clicked, this, &CredentialMainView::OnClickedMotifyName);
    QObject::connect(_ui.m_btnMotifyWord, &QPushButton::clicked, this, &CredentialMainView::OnClickedMotifyWord);
}

bool CredentialMainView::SaveCredential() const
{
    if (m_Credential.GetWord().empty()) return false;
    if (m_strFile.isEmpty()) return false;

    return m_Credential.Save(m_strFile.toStdString().c_str());
}

void CredentialMainView::UpdateCredentail()
{
    CredentialView * view = new CredentialView(m_Credential, this, this);

    QWidget* old = _ui.m_areaCredential->takeWidget();
    if (nullptr != old) delete old;

    _ui.m_areaCredential->setWidget(view);
    _ui.m_areaCredential->setMaximumSize(view->width() + 20, view->height() + 2);

    if (!_ui.m_btnMotifyName->isEnabled()) _ui.m_btnMotifyName->setEnabled(true);
    if (!_ui.m_btnMotifyWord->isEnabled()) _ui.m_btnMotifyWord->setEnabled(true);

    UpdateSize();
}

void CredentialMainView::UpdateSize()
{
    setFixedWidth(_ui.m_areaCredential->maximumWidth() + 8);
    setMaximumHeight(_ui.m_areaCredential->maximumHeight() + 32);
}

void CredentialMainView::UpdateTitle()
{
    setWindowTitle("Credential - " + QString::fromStdString(m_Credential.GetUser()) + " [" + m_strFile + "]");
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
        m_Credential.Tree().Insert(platform);
        UpdateCredentail();
        SaveCredential();
        return true;
    }

    return false;
}

bool CredentialMainView::OnAddAccount(bnb::platform_type* pp)
{
    auto ptr_platform = m_Credential.Tree().Find(*pp);
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
    if (m_Credential.Tree().Remove(*pp))
    {
        UpdateCredentail();
        SaveCredential();
        return true;
    }
    
    return false;
}

bool CredentialMainView::OnRemoveAccount(bnb::platform_type * pp, bnb::account_type * pa)
{
    auto ptr_platform = m_Credential.Tree().Find(*pp);
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

bool CredentialMainView::OnViewCredential(bnb::platform_type * pp, bnb::account_type * pa)
{
    CredentialDialog dlg(m_Credential, this);

    dlg.exec();

    return true;
}

bool CredentialMainView::SetPlatform(bnb::platform_type & p1, const bnb::platform_type & p2)
{
    return m_Credential.Tree().Update(p1, p2);
}

bool CredentialMainView::SetAccount(const bnb::platform_type & pp, bnb::account_type & a1, const bnb::account_type & a2)
{
    auto ptr_platform = m_Credential.Tree().Find(pp);
    if (ptr_platform)
        return ptr_platform->m_Value.Update(a1, a2);

    return false;
}

bool CredentialMainView::SetKey(const bnb::platform_type & pp, const bnb::account_type & pa, bnb::string_type & k1, const bnb::string_type & k2)
{
    auto ptr_platform = m_Credential.Tree().Find(pp);
    if (ptr_platform)
    {
        auto ptr_account = ptr_platform->m_Value.Find(pa);
        if (ptr_account)
            return ptr_account->m_Value.Update(k1, k2);
    }

    return false;



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

    m_btnNew = ui_utils::MakeButton(pView);
    m_btnOpen = ui_utils::MakeButton(pView);

    m_btnMotifyName = new QPushButton(pView);
    m_btnMotifyName->setFixedHeight(20);
    m_btnMotifyWord = new QPushButton(pView);
    m_btnMotifyWord->setFixedHeight(20);

    m_btnMotifyName->setEnabled(false);
    m_btnMotifyWord->setEnabled(false);

    QHBoxLayout* phLayout1 = new QHBoxLayout;
    phLayout1->setSpacing(8);
    phLayout1->setMargin(0);
    phLayout1->addWidget(m_btnNew);
    phLayout1->addWidget(m_btnOpen);
    phLayout1->addStretch(1);
    phLayout1->addWidget(m_btnMotifyName);
    phLayout1->addWidget(m_btnMotifyWord);

    QLabel* labHint = new QLabel("Please new or open a credential file !");
    labHint->setAlignment(Qt::AlignCenter);
    labHint->setFixedSize(400, 30);

    m_areaCredential = new QScrollArea(pView);
    m_areaCredential->setWidget(labHint);
    m_areaCredential->setMaximumSize(420, 32);
    m_areaCredential->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_areaCredential->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QVBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->setSpacing(4);
    pMainLayout->setMargin(4);

    pMainLayout->addLayout(phLayout1);
    pMainLayout->addWidget(m_areaCredential, 1);

    pView->setLayout(pMainLayout);

    RetranslateUI(pView);
}

void CredentialMainView::ui_type::RetranslateUI(CredentialMainView * pView)
{
    m_btnNew->setText("New");
    m_btnOpen->setText("Open");
    m_btnMotifyName->setText("Motify User Name");
    m_btnMotifyWord->setText("Motify Password");
}

QT_END_NAMESPACE
