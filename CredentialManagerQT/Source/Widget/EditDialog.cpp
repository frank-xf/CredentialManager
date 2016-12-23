#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

#include "Credential/Credential.h"

#include "credential_qt_string.h"
#include "credential_qt_utils.h"

#include "Widget/EditDialog.h"

static inline bool IsEqual(const bnb::string_type& left, const bnb::string_type& right)
{
    return 0 == wcscmp(left.c_str(), right.c_str());
}

//==============================================================================
// Implementation of EditCredentialDialog
//==============================================================================
EditCredentialDialog::EditCredentialDialog(bnb::Credential& pc, QWidget * parent)
    : base_type(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
    , m_Credential(pc)
{
    setObjectName("EditCredentialDialog");

    _ui.m_editText[0]->setFocus();
    _ui.m_editText[0]->setMaxLength(ui_utils::def_text_length);
    _ui.m_editText[0]->setText(To_QString(m_Credential.GetData().GetUser()));
    _ui.m_editText[1]->setText(To_QString(m_Credential.GetData().GetComment()));
}

void EditCredentialDialog::OnClickedOK()
{
    if (_ui.m_editText[0]->text().isEmpty())
    {
        _ui.m_labHint->setText("User name mustn\'t be null !");
        return;
    }

    if (!bnb::Credential::ValidateName(From_QString(_ui.m_editText[0]->text())))
    {
        _ui.m_labHint->setText("The user name you entered is invalid !");
        return;
    }

    auto strUserName(From_QString(_ui.m_editText[0]->text()));
    auto strComment(From_QString(_ui.m_editText[1]->text()));
    if (IsEqual(m_Credential.GetData().GetUser(), strUserName)
        && IsEqual(m_Credential.GetData().GetComment(), strComment))
    {
        reject();
        return;
    }

    m_Credential.SetUser(strUserName);
    m_Credential.SetComment(strComment);

    accept();
}

//------------------------------------------------------------------------------

template<>
QVBoxLayout* EditCredentialDialog::base_type::ui_type::LayoutLayoutMarkLabel(EditCredentialDialog::base_type* pView)
{
    QVBoxLayout* pvLayout = new QVBoxLayout;
    pvLayout->setMargin(0);
    pvLayout->setSpacing(2);
    pvLayout->addWidget(ui_utils::MakeMarkLabel(pView));
    pvLayout->addStretch(1);

    return pvLayout;
}

template<>
void EditCredentialDialog::base_type::ui_type::RetranslateLabel(EditCredentialDialog::base_type * pView)
{
    _labText[0]->setText("User Name: ");
    _labText[1]->setText("Description: ");

    m_editText[0]->setPlaceholderText("input your name");
    m_editText[1]->setPlaceholderText("input a description");
}

//==============================================================================
// Implementation of EditPasswordDialog
//==============================================================================
EditPasswordDialog::EditPasswordDialog(bnb::Credential& pc, QWidget * parent)
    : base_type(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
    , m_Credential(pc)
{
    setObjectName("EditPasswordDialog");

    _ui.m_editText[0]->setFocus();

    for (unsigned int i = 0; i < 3; i++)
    {
        _ui.m_editText[i]->setMaxLength(ui_utils::def_text_length);
    }
}

void EditPasswordDialog::OnClickedOK()
{
    if (_ui.m_editText[0]->text().isEmpty())
    {
        _ui.m_labHint->setText("Old password mustn\'t be null !");
        return;
    }

    if (_ui.m_editText[1]->text().isEmpty())
    {
        _ui.m_labHint->setText("New password mustn\'t be null !");
        return;
    }

    if (_ui.m_editText[1]->text() != _ui.m_editText[2]->text())
    {
        _ui.m_labHint->setText("The two passwords you entered didn\'t match !");
        return;
    }

    if (!m_Credential.GetData().ValidateWord(From_QString(_ui.m_editText[0]->text())))
    {
        _ui.m_labHint->setText("The old password you entered is incorrect !");
        return;
    }

    m_Credential.SetWord(From_QString(_ui.m_editText[1]->text()));

    accept();
}

//------------------------------------------------------------------------------

template<>
void EditPasswordDialog::base_type::ui_type::CreateLabel(EditPasswordDialog::base_type * pView)
{
    for (unsigned int i = 0; i < 3; ++i)
    {
        _labText[i] = ui_utils::MakeStaticLabel(pView);
        m_editText[i] = ui_utils::MakePasswordLine(pView, ui_utils::edit_password_w);
    }
}

template<>
QVBoxLayout* EditPasswordDialog::base_type::ui_type::LayoutLayoutMarkLabel(EditPasswordDialog::base_type* pView)
{
    QVBoxLayout* pvLayout = new QVBoxLayout;
    pvLayout->setMargin(0);
    pvLayout->setSpacing(2);
    pvLayout->addWidget(ui_utils::MakeMarkLabel(pView));
    pvLayout->addWidget(ui_utils::MakeMarkLabel(pView));
    pvLayout->addWidget(ui_utils::MakeMarkLabel(pView));

    return pvLayout;
}

template<>
void EditPasswordDialog::base_type::ui_type::RetranslateLabel(EditPasswordDialog::base_type * pView)
{
    _labText[0]->setText("Old Password: ");
    _labText[1]->setText("New Password: ");
    _labText[2]->setText("Validate: ");

    m_editText[0]->setPlaceholderText("input the old password");
    m_editText[1]->setPlaceholderText("input the new password");
    m_editText[2]->setPlaceholderText("input the new password again");
}

//==============================================================================
// Implementation of EditPlatformDialog
//==============================================================================
EditPlatformDialog::EditPlatformDialog(bnb::Credential& pc, const bnb::platform_node* pp, QWidget* parent)
    : base_type(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
    , m_Credential(pc)
    , m_Platform(pp)
{
    setObjectName("EditPlatformDialog");

    _ui.m_editText[0]->setFocus();
    _ui.m_editText[0]->setMaxLength(ui_utils::def_text_length);

    if (m_Platform)
    {
        if (!m_Platform->GetData().GetName().empty())
            _ui.m_editText[0]->setText(To_QString(m_Platform->GetData().GetName()));
        if (!m_Platform->GetData().GetUrl().empty())
            _ui.m_editText[1]->setText(To_QString(m_Platform->GetData().GetUrl()));
        if (!m_Platform->GetData().GetComment().empty())
            _ui.m_editText[2]->setText(To_QString(m_Platform->GetData().GetComment()));

        setWindowTitle("Edit Platform");
    }
    else
    {
        setWindowTitle("PushBack Platform");
    }
}

const bnb::platform_node * EditPlatformDialog::GetPlatform() const
{
    return m_Platform;
}

void EditPlatformDialog::OnClickedOK()
{
    if (_ui.m_editText[0]->text().isEmpty())
    {
        _ui.m_labHint->setText("Platform name mustn\'t be null !");
        return;
    }

    if (!bnb::Credential::ValidateName(From_QString(_ui.m_editText[0]->text())))
    {
        _ui.m_labHint->setText("The platform you entered is invalid !");
        return;
    }

    auto strPlatform(From_QString(_ui.m_editText[0]->text()));
    auto strUrl(From_QString(_ui.m_editText[1]->text()));
    auto strComment(From_QString(_ui.m_editText[2]->text()));

    bnb::platform_type platform(strPlatform, strUrl, strComment);

    if (m_Platform)
    {
        if (IsEqual(m_Platform->GetData().GetName(), strPlatform)
            && IsEqual(m_Platform->GetData().GetUrl(), strUrl)
            && IsEqual(m_Platform->GetData().GetComment(), strComment))
        {
            reject();
            return;
        }

        if (!m_Credential.Update(m_Platform->GetData(), platform))
        {
            _ui.m_labHint->setText("The platform name you entered already exists or is invalid !");
            return;
        }
    }
    else
    {
        m_Platform = m_Credential.PushBack(platform);
        if (nullptr == m_Platform)
        {
            _ui.m_labHint->setText("The platform name you entered already exists or is invalid !");
            return;
        }
    }

    accept();
}

//------------------------------------------------------------------------------

template<>
QVBoxLayout* EditPlatformDialog::base_type::ui_type::LayoutLayoutMarkLabel(EditPlatformDialog::base_type* pView)
{
    QVBoxLayout* pvLayout = new QVBoxLayout;
    pvLayout->setMargin(0);
    pvLayout->setSpacing(2);
    pvLayout->addWidget(ui_utils::MakeMarkLabel(pView));
    pvLayout->addStretch(1);

    return pvLayout;
}

template<>
void EditPlatformDialog::base_type::ui_type::RetranslateLabel(EditPlatformDialog::base_type * pView)
{
    _labText[0]->setText("Platform: ");
    _labText[1]->setText("Url: ");
    _labText[2]->setText("Description: ");

    m_editText[0]->setPlaceholderText("input a platform");
    m_editText[1]->setPlaceholderText("input a url of the platform");
    m_editText[2]->setPlaceholderText("input a description of the platform");
}

//==============================================================================
// Implementation of EditAccountDialog
//==============================================================================
EditAccountDialog::EditAccountDialog(bnb::platform_node& pp, const bnb::account_node* pa, QWidget * parent)
    : base_type(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
    , m_Platform(pp)
    , m_Account(pa)
{
    setObjectName("EditAccountDialog");

    _ui.m_editText[0]->setReadOnly(true);
    _ui.m_editText[1]->setFocus();
    _ui.m_editText[1]->setMaxLength(ui_utils::def_text_length);

    if (!m_Platform.GetData().GetName().empty())
        _ui.m_editText[0]->setText(To_QString(m_Platform.GetData().GetName()));

    if (m_Account)
    {
        if (!m_Account->GetData().GetName().empty())
            _ui.m_editText[1]->setText(To_QString(m_Account->GetData().GetName()));

        if (!m_Account->GetData().GetComment().empty())
            _ui.m_editText[2]->setText(To_QString(m_Account->GetData().GetComment()));

        setWindowTitle("Edit Account");
    }
    else
    {
        setWindowTitle("PushBack Account");
    }
}

const bnb::account_node * EditAccountDialog::GetAccount() const
{
    return m_Account;
}

void EditAccountDialog::OnClickedOK()
{
    if (_ui.m_editText[1]->text().isEmpty())
    {
        _ui.m_labHint->setText("Account name mustn\'t be null !");
        return;
    }

    if (!bnb::Credential::ValidateName(From_QString(_ui.m_editText[1]->text())))
    {
        _ui.m_labHint->setText("The account you entered is invalid !");
        return;
    }

    auto strAccount(From_QString(_ui.m_editText[1]->text()));
    auto strComment(From_QString(_ui.m_editText[2]->text()));
    bnb::account_type account(strAccount, strComment);

    if (m_Account)
    {
        if (IsEqual(m_Account->GetData().GetName(), strAccount) && IsEqual(m_Account->GetData().GetComment(), strComment))
        {
            reject();
            return;
        }

        if (!m_Platform.Update(m_Account->GetData(), account))
        {
            _ui.m_labHint->setText("The account name you entered already exists or is invalid !");
            return;
        }
    }
    else
    {
        m_Account = m_Platform.PushBack(account);
        if (nullptr == m_Account)
        {
            _ui.m_labHint->setText("The account name you entered already exists or is invalid !");
            return;
        }
    }

    accept();
}

//------------------------------------------------------------------------------

template<>
QVBoxLayout* EditAccountDialog::base_type::ui_type::LayoutLayoutMarkLabel(EditAccountDialog::base_type* pView)
{
    QVBoxLayout* pvLayout = new QVBoxLayout;
    pvLayout->setMargin(0);
    pvLayout->setSpacing(2);
    pvLayout->addStretch(1);
    pvLayout->addWidget(ui_utils::MakeMarkLabel(pView));
    pvLayout->addStretch(1);

    return pvLayout;
}

template<>
void EditAccountDialog::base_type::ui_type::RetranslateLabel(EditAccountDialog::base_type * pView)
{
    _labText[0]->setText("Platform: ");
    _labText[1]->setText("Account: ");
    _labText[2]->setText("Description: ");

    m_editText[1]->setPlaceholderText("input a account");
    m_editText[2]->setPlaceholderText("input a description of the account");
}

//==============================================================================
// Implementation of EditPairDialog
//==============================================================================
EditPairDialog::EditPairDialog(bnb::account_node & pa, const bnb::pair_node * pp, QWidget * parent)
    : base_type(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
    , m_Account(pa)
    , m_Pair(pp)
{
    setObjectName("EditPairDialog");

    _ui.m_editText[0]->setReadOnly(true);
    _ui.m_editText[1]->setFocus();
    _ui.m_editText[1]->setMaxLength(ui_utils::def_text_length << 1);

    if (!m_Account.GetData().GetName().empty())
        _ui.m_editText[0]->setText(To_QString(m_Account.GetData().GetName()));

    if (m_Pair)
    {
        if (!m_Pair->GetData().GetKey().empty())
            _ui.m_editText[1]->setText(To_QString(m_Pair->GetData().GetKey()));

        if (!m_Pair->GetData().GetValue().empty())
            _ui.m_editText[2]->setText(To_QString(m_Pair->GetData().GetValue()));

        setWindowTitle("Edit Pair");
    }
    else
    {
        setWindowTitle("Add Pair");
    }
}

const bnb::pair_node * EditPairDialog::GetPair() const
{
    return m_Pair;
}

void EditPairDialog::OnClickedOK()
{
    if (_ui.m_editText[1]->text().isEmpty())
    {
        _ui.m_labHint->setText("The key mustn\'t be null !");
        return;
    }

    if (!bnb::Credential::ValidateName(From_QString(_ui.m_editText[1]->text())))
    {
        _ui.m_labHint->setText("The key you entered is invalid !");
        return;
    }

    auto key(From_QString(_ui.m_editText[1]->text()));
    auto value(From_QString(_ui.m_editText[2]->text()));
    bnb::pair_type pair(key, value);

    if (m_Pair)
    {
        if (IsEqual(m_Pair->GetData().GetKey(), key) && IsEqual(m_Pair->GetData().GetValue(), value))
        {
            reject();
            return;
        }

        if (!m_Account.Update(m_Pair->GetData(), pair))
        {
            _ui.m_labHint->setText("The key you entered already exists or is invalid !");
            return;
        }
    }
    else
    {
        m_Pair = m_Account.PushBack(pair);
        if (nullptr == m_Pair)
        {
            _ui.m_labHint->setText("The key you entered already exists or is invalid !");
            return;
        }
    }

    accept();
}

//------------------------------------------------------------------------------

template<>
QVBoxLayout* EditPairDialog::base_type::ui_type::LayoutLayoutMarkLabel(EditPairDialog::base_type* pView)
{
    QVBoxLayout* pvLayout = new QVBoxLayout;
    pvLayout->setMargin(0);
    pvLayout->setSpacing(2);
    pvLayout->addStretch(1);
    pvLayout->addWidget(ui_utils::MakeMarkLabel(pView));
    pvLayout->addStretch(1);

    return pvLayout;
}

template<>
void EditPairDialog::base_type::ui_type::RetranslateLabel(EditPairDialog::base_type * pView)
{
    _labText[0]->setText("Account: ");
    _labText[1]->setText("Key: ");
    _labText[2]->setText("Value: ");

    m_editText[1]->setPlaceholderText("input a key");
    m_editText[2]->setPlaceholderText("input a value");
}
