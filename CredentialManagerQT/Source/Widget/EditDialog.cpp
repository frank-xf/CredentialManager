#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

#include "Credential/Credential.h"

#include "credential_qt_utils.h"
#include "credential_qt_manager.h"
#include "credential_model_manager.h"

#include "Widget/EditDialog.h"

//==============================================================================
// Implementation of EditCredentialDialog
//==============================================================================
EditCredentialDialog::EditCredentialDialog(bnb::Credential& pc, QWidget * parent)
    : base_type(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
    , m_Credential(pc)
{
    setObjectName("EditCredentialDialog");

    _ui.m_editText[0]->setMaxLength(64);
    _ui.m_editText[0]->setText(QString::fromStdString(m_Credential.GetUser()));
    _ui.m_editText[1]->setText(QString::fromStdString(m_Credential.GetDisplay()));
}

void EditCredentialDialog::OnClickedOK()
{
    if (_ui.m_editText[0]->text().isEmpty())
    {
        _ui.m_labHint->setText("User name mustn\'t be null !");
        return;
    }

    if (!true)
    {
        _ui.m_labHint->setText("The user name you entered is invalid !");
        return;
    }

    m_Credential.SetUser(_ui.m_editText[0]->text().toStdString());
    m_Credential.SetDisplay(_ui.m_editText[1]->text().toStdString());

    accept();
}

//------------------------------------------------------------------------------

template<>
void EditCredentialDialog::base_type::ui_type::RetranslateLabel(EditCredentialDialog::base_type * pView)
{
    _labText[0]->setText("User Name: ");
    _labText[1]->setText("Display: ");
}

//==============================================================================
// Implementation of EditPasswordDialog
//==============================================================================
EditPasswordDialog::EditPasswordDialog(bnb::Credential& pc, QWidget * parent)
    : base_type(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
    , m_Credential(pc)
{
    setObjectName("EditPasswordDialog");

    _ui.m_editText[0]->setMaxLength(64);
    _ui.m_editText[0]->setEchoMode(QLineEdit::Password);
    _ui.m_editText[1]->setMaxLength(64);
    _ui.m_editText[1]->setEchoMode(QLineEdit::Password);
    _ui.m_editText[2]->setMaxLength(64);
    _ui.m_editText[2]->setEchoMode(QLineEdit::Password);
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

    if (!m_Credential.ValidateWord(_ui.m_editText[0]->text().toStdString()))
    {
        _ui.m_labHint->setText("The old password you entered is incorrect !");
        return;
    }

    m_Credential.SetWord(_ui.m_editText[1]->text().toStdString());

    accept();
}

//------------------------------------------------------------------------------

template<>
void EditPasswordDialog::base_type::ui_type::LayoutCentral(EditPasswordDialog::base_type* pView, QBoxLayout * pMainLayout)
{
    for (unsigned int i = 0; i < 3; ++i)
    {
        QHBoxLayout* phLayout = new QHBoxLayout;
        phLayout->setMargin(0);
        phLayout->setSpacing(0);
        phLayout->addWidget(_labText[i]);
        phLayout->addWidget(m_editText[i], 1);

        pMainLayout->addLayout(phLayout);
    }
}

template<>
void EditPasswordDialog::base_type::ui_type::RetranslateLabel(EditPasswordDialog::base_type * pView)
{
    _labText[0]->setText("Old Password: ");
    _labText[1]->setText("New Password: ");
    _labText[2]->setText("Validate: ");
}

//==============================================================================
// Implementation of EditPlatformDialog
//==============================================================================
EditPlatformDialog::EditPlatformDialog(bnb::Credential& pc, bnb::platform_tree::data_type* pp, QWidget* parent)
    : base_type(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
    , m_Credential(pc)
    , m_Platform(pp)
{
    setObjectName("EditPlatformDialog");

    _ui.m_editText[0]->setMaxLength(64);

    if (m_Platform)
    {
        if (!m_Platform->m_Key.m_strName.empty())
            _ui.m_editText[0]->setText(QString::fromStdString(m_Platform->m_Key.m_strName));
        if (!m_Platform->m_Key.m_strUrl.empty())
            _ui.m_editText[1]->setText(QString::fromStdString(m_Platform->m_Key.m_strUrl));
        if (!m_Platform->m_Key.m_strDisplay.empty())
            _ui.m_editText[2]->setText(QString::fromStdString(m_Platform->m_Key.m_strDisplay));

        setWindowTitle("Edit Platform");
    }
    else
    {
        setWindowTitle("Add Platform");
    }
}

const bnb::platform_tree::data_type * EditPlatformDialog::GetPlatform() const
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

    bnb::platform_type platform(
        _ui.m_editText[0]->text().toStdString(),
        _ui.m_editText[1]->text().toStdString(),
        _ui.m_editText[2]->text().toStdString());

    if (m_Platform)
    {
        if (!m_Credential.Tree().Update(m_Platform->m_Key, platform))
        {
            _ui.m_labHint->setText("The platform name you entered already exists or is invalid !");
            return;
        }
    }
    else
    {
        m_Platform = m_Credential.Tree().Insert(platform);
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
void EditPlatformDialog::base_type::ui_type::RetranslateLabel(EditPlatformDialog::base_type * pView)
{
    _labText[0]->setText("Platform: ");
    _labText[1]->setText("Url: ");
    _labText[2]->setText("Display: ");
}

//==============================================================================
// Implementation of EditAccountDialog
//==============================================================================
EditAccountDialog::EditAccountDialog(bnb::platform_tree::data_type& pp, bnb::account_tree::data_type* pa, QWidget * parent)
    : base_type(parent, 0)
    , m_Platform(pp)
    , m_Account(pa)
{
    setObjectName("EditAccountDialog");

    _ui.m_editText[0]->setReadOnly(true);
    _ui.m_editText[1]->setMaxLength(64);

    if (!m_Platform.m_Key.m_strName.empty())
        _ui.m_editText[0]->setText(QString::fromStdString(m_Platform.m_Key.m_strName));

    if (m_Account)
    {
        if (!m_Account->m_Key.m_strName.empty())
            _ui.m_editText[1]->setText(QString::fromStdString(m_Account->m_Key.m_strName));

        if (!m_Account->m_Key.m_strDisplay.empty())
            _ui.m_editText[2]->setText(QString::fromStdString(m_Account->m_Key.m_strDisplay));

        setWindowTitle("Edit Account");
    }
    else
    {
        setWindowTitle("Add Account");
    }
}

const bnb::account_tree::data_type * EditAccountDialog::GetAccount() const
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

    bnb::account_type account(_ui.m_editText[1]->text().toStdString(), _ui.m_editText[2]->text().toStdString());

    if (m_Account)
    {
        if (!m_Platform.m_Value.Update(m_Account->m_Key, account))
        {
            _ui.m_labHint->setText("The account name you entered already exists or is invalid !");
            return;
        }
    }
    else
    {
        m_Account = m_Platform.m_Value.Insert(account);
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
void EditAccountDialog::base_type::ui_type::LayoutCentral(EditAccountDialog::base_type* pView, QBoxLayout * pMainLayout)
{
    for (unsigned int i = 0; i < 3; ++i)
    {
        QHBoxLayout* phLayout = new QHBoxLayout;
        phLayout->setMargin(0);
        phLayout->setSpacing(0);
        phLayout->addWidget(_labText[i]);
        phLayout->addWidget(m_editText[i], 1);

        pMainLayout->addLayout(phLayout);
    }
}

template<>
void EditAccountDialog::base_type::ui_type::RetranslateLabel(EditAccountDialog::base_type * pView)
{
    _labText[0]->setText("Platform: ");
    _labText[1]->setText("Account: ");
    _labText[2]->setText("Display: ");
}

//==============================================================================
// Implementation of EditPropertyDialog
//==============================================================================
EditPropertyDialog::EditPropertyDialog(bnb::account_tree::data_type & pa, bnb::property_tree::data_type * pp, QWidget * parent)
    : base_type(parent, 0)
    , m_Account(pa)
    , m_Property(pp)
{
    setObjectName("EditPropertyDialog");

    _ui.m_editText[0]->setReadOnly(true);
    _ui.m_editText[1]->setMaxLength(128);

    if (!m_Account.m_Key.m_strName.empty())
        _ui.m_editText[0]->setText(QString::fromStdString(m_Account.m_Key.m_strName));

    if (m_Property)
    {
        if (!m_Property->m_Key.m_strName.empty())
            _ui.m_editText[1]->setText(QString::fromStdString(m_Property->m_Key.m_strName));

        if (!m_Property->m_Value.m_strName.empty())
            _ui.m_editText[2]->setText(QString::fromStdString(m_Property->m_Value.m_strName));

        setWindowTitle("Edit Property");
    }
    else
    {
        setWindowTitle("Add Property");
    }
}

const bnb::property_tree::data_type * EditPropertyDialog::GetProperty() const
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

    bnb::property_key key(_ui.m_editText[1]->text().toStdString());
    bnb::property_value value(_ui.m_editText[2]->text().toStdString());

    if (m_Property)
    {
        if (!m_Account.m_Value.Update(m_Property->m_Key, key, value))
        {
            _ui.m_labHint->setText("The account name you entered already exists or is invalid !");
            return;
        }
    }
    else
    {
        m_Property = m_Account.m_Value.Insert(key, value);
        if (nullptr == m_Property)
        {
            _ui.m_labHint->setText("The account name you entered already exists or is invalid !");
            return;
        }
    }

    accept();
}

//------------------------------------------------------------------------------

template<>
void EditPropertyDialog::base_type::ui_type::LayoutCentral(EditPropertyDialog::base_type* pView, QBoxLayout * pMainLayout)
{
    for (unsigned int i = 0; i < 3; ++i)
    {
        QHBoxLayout* phLayout = new QHBoxLayout;
        phLayout->setMargin(0);
        phLayout->setSpacing(0);
        phLayout->addWidget(_labText[i]);
        phLayout->addWidget(m_editText[i], 1);

        pMainLayout->addLayout(phLayout);
    }
}

template<>
void EditPropertyDialog::base_type::ui_type::RetranslateLabel(EditPropertyDialog::base_type * pView)
{
    _labText[0]->setText("Account: ");
    _labText[1]->setText("Key: ");
    _labText[2]->setText("Value: ");
}
