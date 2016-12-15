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

    if (!ui_utils::ValidateName(_ui.m_editText[0]->text()))
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

void EditCredentialDialog::base_type::ui_type::LayoutCentral(EditCredentialDialog::base_type* pView, QBoxLayout* pMainLayout)
{
    for (unsigned int i = 0; i < 2; ++i)
    {
        QHBoxLayout* phLayout = new QHBoxLayout;
        phLayout->setMargin(0);
        phLayout->setSpacing(0);
        phLayout->addWidget(_labText[i]);
        phLayout->addWidget(m_editText[i]);

        if (0 == i)
            phLayout->addWidget(ui_utils::MakeMarkLabel(pView));
        else
            phLayout->addStretch(1);

        pMainLayout->addLayout(phLayout);
    }
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
        _labText[i] = ui_utils::MakeStaticLabel(pView, ui_utils::lab_password_w);
        m_editText[i] = ui_utils::MakePasswordLine(pView, ui_utils::edit_password_w);
    }
}

template<>
void EditPasswordDialog::base_type::ui_type::LayoutCentral(EditPasswordDialog::base_type* pView, QBoxLayout * pMainLayout)
{
    for (unsigned int i = 0; i < 3; ++i)
    {
        QHBoxLayout* phLayout = new QHBoxLayout;
        phLayout->setMargin(0);
        phLayout->setSpacing(0);
        phLayout->addWidget(_labText[i]);
        phLayout->addWidget(m_editText[i]);
        phLayout->addWidget(ui_utils::MakeMarkLabel(pView));

        pMainLayout->addLayout(phLayout);
    }
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
EditPlatformDialog::EditPlatformDialog(bnb::Credential& pc, const bnb::platform_type* pp, QWidget* parent)
    : base_type(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
    , m_Credential(pc)
    , m_Platform(pp)
{
    setObjectName("EditPlatformDialog");

    _ui.m_editText[0]->setFocus();
    _ui.m_editText[0]->setMaxLength(ui_utils::def_text_length);

    if (m_Platform)
    {
        if (!m_Platform->GetName().empty())
            _ui.m_editText[0]->setText(To_QString(m_Platform->GetName()));
        if (!m_Platform->GetUrl().empty())
            _ui.m_editText[1]->setText(To_QString(m_Platform->GetUrl()));
        if (!m_Platform->GetComment().empty())
            _ui.m_editText[2]->setText(To_QString(m_Platform->GetComment()));

        setWindowTitle("Edit Platform");
    }
    else
    {
        setWindowTitle("PushBack Platform");
    }
}

const bnb::platform_type * EditPlatformDialog::GetPlatform() const
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

    if (!ui_utils::ValidateName(_ui.m_editText[0]->text()))
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
        if (IsEqual(m_Platform->GetName(), strPlatform)
            && IsEqual(m_Platform->GetUrl(), strUrl)
            && IsEqual(m_Platform->GetComment(), strComment))
        {
            reject();
            return;
        }

        if (!m_Credential.Update(*m_Platform, platform))
        {
            _ui.m_labHint->setText("The platform name you entered already exists or is invalid !");
            return;
        }
    }
    else
    {
        auto ptr = m_Credential.PushBack(platform);
        if (nullptr == ptr)
        {
            _ui.m_labHint->setText("The platform name you entered already exists or is invalid !");
            return;
        }

        m_Platform = &ptr->GetData();
    }

    accept();
}

//------------------------------------------------------------------------------

void EditPlatformDialog::base_type::ui_type::LayoutCentral(EditPlatformDialog::base_type* pView, QBoxLayout* pMainLayout)
{
    for (unsigned int i = 0; i < 3; ++i)
    {
        QHBoxLayout* phLayout = new QHBoxLayout;
        phLayout->setMargin(0);
        phLayout->setSpacing(0);
        phLayout->addWidget(_labText[i]);
        phLayout->addWidget(m_editText[i]);

        if (0 == i)
            phLayout->addWidget(ui_utils::MakeMarkLabel(pView));
        else
            phLayout->addStretch(1);

        pMainLayout->addLayout(phLayout);
    }
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
EditAccountDialog::EditAccountDialog(bnb::platform_node& pp, const bnb::account_type* pa, QWidget * parent)
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
        if (!m_Account->GetName().empty())
            _ui.m_editText[1]->setText(To_QString(m_Account->GetName()));

        if (!m_Account->GetComment().empty())
            _ui.m_editText[2]->setText(To_QString(m_Account->GetComment()));

        setWindowTitle("Edit Account");
    }
    else
    {
        setWindowTitle("PushBack Account");
    }
}

const bnb::account_type * EditAccountDialog::GetAccount() const
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

    if (!ui_utils::ValidateName(_ui.m_editText[1]->text()))
    {
        _ui.m_labHint->setText("The account you entered is invalid !");
        return;
    }

    auto strAccount(From_QString(_ui.m_editText[1]->text()));
    auto strComment(From_QString(_ui.m_editText[2]->text()));
    bnb::account_type account(strAccount, strComment);

    if (m_Account)
    {
        if (IsEqual(m_Account->GetName(), strAccount) && IsEqual(m_Account->GetComment(), strComment))
        {
            reject();
            return;
        }

        if (!m_Platform.Update(*m_Account, account))
        {
            _ui.m_labHint->setText("The account name you entered already exists or is invalid !");
            return;
        }
    }
    else
    {
        auto ptr = m_Platform.PushBack(account);
        if (nullptr == ptr)
        {
            _ui.m_labHint->setText("The account name you entered already exists or is invalid !");
            return;
        }

        m_Account = &ptr->GetData();
    }

    accept();
}

//------------------------------------------------------------------------------

template<>
void EditAccountDialog::base_type::ui_type::LayoutCentral(EditAccountDialog::base_type* pView, QBoxLayout * pMainLayout)
{
    for (unsigned int i = 0; i < 3; ++i)
    {
        QHBoxLayout* phLayout = new QHBoxLayout;
        phLayout->setMargin(0);
        phLayout->setSpacing(0);
        phLayout->addWidget(_labText[i]);
        phLayout->addWidget(m_editText[i]);

        if (1 == i)
            phLayout->addWidget(ui_utils::MakeMarkLabel(pView));
        else
            phLayout->addStretch(1);

        pMainLayout->addLayout(phLayout);
    }
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
// Implementation of EditPropertyDialog
//==============================================================================
EditPropertyDialog::EditPropertyDialog(bnb::account_node & pa, const bnb::property_type * pp, QWidget * parent)
    : base_type(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
    , m_Account(pa)
    , m_Property(pp)
{
    setObjectName("EditPropertyDialog");

    _ui.m_editText[0]->setReadOnly(true);
    _ui.m_editText[1]->setFocus();
    _ui.m_editText[1]->setMaxLength(ui_utils::def_text_length << 1);

    if (!m_Account.GetData().GetName().empty())
        _ui.m_editText[0]->setText(To_QString(m_Account.GetData().GetName()));

    if (m_Property)
    {
        if (!m_Property->GetKey().empty())
            _ui.m_editText[1]->setText(To_QString(m_Property->GetKey()));

        if (!m_Property->GetValue().empty())
            _ui.m_editText[2]->setText(To_QString(m_Property->GetValue()));

        setWindowTitle("Edit Property");
    }
    else
    {
        setWindowTitle("PushBack Property");
    }
}

const bnb::property_type * EditPropertyDialog::GetProperty() const
{
    return m_Property;
}

void EditPropertyDialog::OnClickedOK()
{
    if (_ui.m_editText[1]->text().isEmpty())
    {
        _ui.m_labHint->setText("Property key mustn\'t be null !");
        return;
    }

    if (!ui_utils::ValidateName(_ui.m_editText[1]->text()))
    {
        _ui.m_labHint->setText("The key you entered is invalid !");
        return;
    }

    auto key(From_QString(_ui.m_editText[1]->text()));
    auto value(From_QString(_ui.m_editText[2]->text()));
    bnb::property_type property(key, value);

    if (m_Property)
    {
        if (IsEqual(m_Property->GetKey(), key) && IsEqual(m_Property->GetValue(), value))
        {
            reject();
            return;
        }

        if (!m_Account.Update(*m_Property, property))
        {
            _ui.m_labHint->setText("The account name you entered already exists or is invalid !");
            return;
        }
    }
    else
    {
        auto ptr = m_Account.PushBack(property);
        if (nullptr == ptr)
        {
            _ui.m_labHint->setText("The account name you entered already exists or is invalid !");
            return;
        }

        m_Property = &ptr->GetData();
    }

    accept();
}

//------------------------------------------------------------------------------

template<>
void EditPropertyDialog::base_type::ui_type::CreateLabel(EditPropertyDialog::base_type * pView)
{
    for (unsigned int i = 0; i < 3; ++i)
    {
        _labText[i] = ui_utils::MakeStaticLabel(pView, ui_utils::lab_account_w);
        m_editText[i] = ui_utils::MakeLineEdit(pView);
    }
}

template<>
void EditPropertyDialog::base_type::ui_type::LayoutCentral(EditPropertyDialog::base_type* pView, QBoxLayout * pMainLayout)
{
    for (unsigned int i = 0; i < 3; ++i)
    {
        QHBoxLayout* phLayout = new QHBoxLayout;
        phLayout->setMargin(0);
        phLayout->setSpacing(0);
        phLayout->addWidget(_labText[i]);
        phLayout->addWidget(m_editText[i]);

        if (1 == i)
            phLayout->addWidget(ui_utils::MakeMarkLabel(pView));
        else
            phLayout->addStretch(1);

        pMainLayout->addLayout(phLayout);
    }
}

template<>
void EditPropertyDialog::base_type::ui_type::RetranslateLabel(EditPropertyDialog::base_type * pView)
{
    _labText[0]->setText("Account: ");
    _labText[1]->setText("Key: ");
    _labText[2]->setText("Value: ");

    m_editText[1]->setPlaceholderText("input a key");
    m_editText[2]->setPlaceholderText("input a value");
}
