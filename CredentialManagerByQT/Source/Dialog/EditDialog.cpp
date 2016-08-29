#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

#include "bnb_global.h"
#include "Credential/Credential.h"

#include "credential_qt_utils.h"
#include "credential_qt_delegate.h"

#include "Dialog/EditDialog.h"

//==============================================================================
// Implementation of EditUserNameDialog
//==============================================================================
EditUserNameDialog::EditUserNameDialog(const QString& strUserName, delegate_type* ptrDelegate, QWidget * parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
    , m_ptrDelegate(ptrDelegate)
{
    _ui.SetupUI(this);

    _ui.m_editUserName->setText(strUserName);

    QObject::connect(_ui.m_editUserName, &QLineEdit::textEdited, this, &EditUserNameDialog::OnChangedText);
    QObject::connect(_ui.m_btnOK, &QPushButton::clicked, this, &EditUserNameDialog::OnClickedOK);
    QObject::connect(_ui.m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

QString EditUserNameDialog::GetUserName() const
{
    return _ui.m_editUserName->text();
}

void EditUserNameDialog::OnChangedText(const QString &)
{
    _ui.m_labHint->clear();
}

void EditUserNameDialog::OnClickedOK()
{
    if (_ui.m_editUserName->text().isEmpty())
    {
        _ui.m_labHint->setText("User name mustn\'t be null !");
        return;
    }

    if (!m_ptrDelegate->ValidateUserName(_ui.m_editUserName->text().toStdString()))
    {
        _ui.m_labHint->setText("The user name you entered is invalid !");
        return;
    }

    accept();
}

//------------------------------------------------------------------------------

void EditUserNameDialog::ui_type::SetupUI(EditUserNameDialog * pView)
{
    pView->setObjectName("EditUserNameDialog");
    pView->setFixedSize(ui_utils::dlg_username_w, ui_utils::dlg_username_h);

    _labUserName = new QLabel(pView);
    _labUserName->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    m_labHint = new QLabel(pView);
    m_labHint->setAlignment(Qt::AlignCenter);
    m_labHint->setFixedHeight(20);

    m_editUserName = new QLineEdit(pView);
    m_editUserName->setMaxLength(64);
    m_editUserName->setFixedSize(ui_utils::edit_default_w, ui_utils::edit_default_h);

    m_btnOK = new QPushButton(pView);
    m_btnCancel = new QPushButton(pView);

    QHBoxLayout* phLayout1 = new QHBoxLayout;
    phLayout1->setMargin(0);
    phLayout1->setSpacing(2);
    phLayout1->addWidget(_labUserName);
    phLayout1->addWidget(m_editUserName, 1);
    phLayout1->addWidget(ui_utils::MakeMarkLabel(pView));

    QHBoxLayout* phLayout2 = new QHBoxLayout;
    phLayout2->setContentsMargins(8, 20, 8, 8);
    phLayout2->addStretch(1);
    phLayout2->addWidget(m_btnOK);
    phLayout2->addStretch(1);
    phLayout2->addWidget(m_btnCancel);
    phLayout2->addStretch(1);

    QVBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->setMargin(4);
    pMainLayout->setSpacing(2);
    pMainLayout->addWidget(m_labHint);
    pMainLayout->addLayout(phLayout1);
    pMainLayout->addLayout(phLayout2, 1);

    pView->setLayout(pMainLayout);

    RetranslateUI(pView);
}

void EditUserNameDialog::ui_type::RetranslateUI(EditUserNameDialog * pView)
{
    _labUserName->setText("User Name:");

    m_btnOK->setText("OK");
    m_btnCancel->setText("Cancel");
}

//==============================================================================
// Implementation of EditPasswordDialog
//==============================================================================
EditPasswordDialog::EditPasswordDialog(delegate_type * ptrDelegate, QWidget * parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
    , m_ptrDelegate(ptrDelegate)
{
    _ui.SetupUI(this);

    QObject::connect(_ui.m_editOldPassword, &QLineEdit::textEdited, this, &EditPasswordDialog::OnChangedText);
    QObject::connect(_ui.m_editNewPassword, &QLineEdit::textEdited, this, &EditPasswordDialog::OnChangedText);
    QObject::connect(_ui.m_editValidate, &QLineEdit::textEdited, this, &EditPasswordDialog::OnChangedText);
    QObject::connect(_ui.m_btnOK, &QPushButton::clicked, this, &EditPasswordDialog::OnClickedOK);
    QObject::connect(_ui.m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

QString EditPasswordDialog::GetPassword() const
{
    return _ui.m_editNewPassword->text();
}

void EditPasswordDialog::OnChangedText(const QString &)
{
    _ui.m_labHint->clear();
}

void EditPasswordDialog::OnClickedOK()
{
    if (_ui.m_editOldPassword->text().isEmpty())
    {
        _ui.m_labHint->setText("Old password mustn\'t be null !");
        return;
    }

    if (_ui.m_editNewPassword->text().isEmpty())
    {
        _ui.m_labHint->setText("New password mustn\'t be null !");
        return;
    }

    if (_ui.m_editNewPassword->text() != _ui.m_editValidate->text())
    {
        _ui.m_labHint->setText("The two passwords you entered didn\'t match !");
        return;
    }

    if (!m_ptrDelegate->ValidatePassword(_ui.m_editOldPassword->text().toStdString()))
    {
        _ui.m_labHint->setText("The old password you entered is incorrect !");
        return;
    }

    accept();
}

//------------------------------------------------------------------------------

void EditPasswordDialog::ui_type::SetupUI(EditPasswordDialog * pView)
{
    pView->setObjectName("EditPasswordDialog");
    pView->setFixedSize(ui_utils::dlg_password_w, ui_utils::dlg_password_h);

    _labOldPassword = new QLabel(pView);
    _labOldPassword->setFixedSize(84, 20);
    _labOldPassword->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    _labNewPassword = new QLabel(pView);
    _labNewPassword->setFixedSize(84, 20);
    _labNewPassword->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    _labValidate = new QLabel(pView);
    _labValidate->setFixedSize(84, 20);
    _labValidate->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    m_labHint = new QLabel(pView);
    m_labHint->setAlignment(Qt::AlignCenter);

    m_editOldPassword = new QLineEdit(pView);
    m_editOldPassword->setMaxLength(64);
    m_editOldPassword->setEchoMode(QLineEdit::Password);
    m_editNewPassword = new QLineEdit(pView);
    m_editNewPassword->setMaxLength(64);
    m_editNewPassword->setEchoMode(QLineEdit::Password);
    m_editValidate = new QLineEdit(pView);
    m_editValidate->setMaxLength(64);
    m_editValidate->setEchoMode(QLineEdit::Password);

    m_btnOK = new QPushButton(pView);
    m_btnCancel = new QPushButton(pView);

    QHBoxLayout* phLayout1 = new QHBoxLayout;
    phLayout1->setMargin(0);
    phLayout1->setSpacing(2);
    phLayout1->addWidget(_labOldPassword);
    phLayout1->addWidget(m_editOldPassword, 1);
    phLayout1->addWidget(ui_utils::MakeMarkLabel(pView));

    QHBoxLayout* phLayout2 = new QHBoxLayout;
    phLayout2->setMargin(0);
    phLayout2->setSpacing(2);
    phLayout2->addWidget(_labNewPassword);
    phLayout2->addWidget(m_editNewPassword, 1);
    phLayout2->addWidget(ui_utils::MakeMarkLabel(pView));

    QHBoxLayout* phLayout3 = new QHBoxLayout;
    phLayout3->setMargin(0);
    phLayout3->setSpacing(2);
    phLayout3->addWidget(_labValidate);
    phLayout3->addWidget(m_editValidate, 1);
    phLayout3->addWidget(ui_utils::MakeMarkLabel(pView));

    QHBoxLayout* phLayout4 = new QHBoxLayout;
    phLayout4->setContentsMargins(10, 20, 10, 8);
    phLayout4->addStretch(1);
    phLayout4->addStretch(1);
    phLayout4->addWidget(m_btnOK);
    phLayout4->addStretch(1);
    phLayout4->addWidget(m_btnCancel);
    phLayout4->addStretch(1);

    QVBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->setMargin(4);
    pMainLayout->setSpacing(4);
    pMainLayout->addWidget(m_labHint);
    pMainLayout->addLayout(phLayout1);
    pMainLayout->addLayout(phLayout2);
    pMainLayout->addLayout(phLayout3);
    pMainLayout->addLayout(phLayout4, 1);

    pView->setLayout(pMainLayout);

    RetranslateUI(pView);
}

void EditPasswordDialog::ui_type::RetranslateUI(EditPasswordDialog * pView)
{
    _labOldPassword->setText("Old Password:");
    _labNewPassword->setText("New Password:");
    _labValidate->setText("Validate:");

    m_btnOK->setText("OK");
    m_btnCancel->setText("Cancel");
}

//==============================================================================
// Implementation of EditPlatformDialog
//==============================================================================
EditPlatformDialog::EditPlatformDialog(bnb::platform_type& platform, delegate_type* ptrDelegate, QWidget* parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
    , m_ptrDelegate(ptrDelegate)
    , m_Platform(platform)
{
    _ui.SetupUI(this);

    if (!m_Platform.m_Key.empty())
        _ui.m_editPlatform->setText(QString::fromStdString(m_Platform.m_Key));

    if (!m_Platform.m_Value.m_strUrl.empty())
        _ui.m_editUrl->setText(QString::fromStdString(m_Platform.m_Value.m_strUrl));

    if (!m_Platform.m_Value.m_strDisplay.empty())
        _ui.m_editDisplay->setText(QString::fromStdString(m_Platform.m_Value.m_strDisplay));

    QObject::connect(_ui.m_editPlatform, &QLineEdit::textEdited, this, &EditPlatformDialog::OnChangedText);
    QObject::connect(_ui.m_editUrl, &QLineEdit::textEdited, this, &EditPlatformDialog::OnChangedText);
    QObject::connect(_ui.m_editDisplay, &QLineEdit::textEdited, this, &EditPlatformDialog::OnChangedText);
    QObject::connect(_ui.m_btnOK, &QPushButton::clicked, this, &EditPlatformDialog::OnClickedOK);
    QObject::connect(_ui.m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

void EditPlatformDialog::OnChangedText(const QString &)
{
    _ui.m_labHint->clear();
}

void EditPlatformDialog::OnClickedOK()
{
    if (_ui.m_editPlatform->text().isEmpty())
    {
        _ui.m_labHint->setText("Platform name mustn\'t be null !");
        return;
    }

    m_Platform.m_Key = _ui.m_editPlatform->text().toStdString();
    m_Platform.m_Value.m_strUrl = _ui.m_editUrl->text().toStdString();
    m_Platform.m_Value.m_strDisplay = _ui.m_editDisplay->text().toStdString();

    if (m_ptrDelegate->CanUpdate(m_Platform))
    {
        accept();
        return;
    }

    _ui.m_labHint->setText("The platform name you entered already exists or is invalid !");
}

//------------------------------------------------------------------------------

void EditPlatformDialog::ui_type::SetupUI(EditPlatformDialog * pView)
{
    pView->setObjectName("EditPlatformDialog");
    pView->setFixedSize(ui_utils::dlg_platform_w, ui_utils::dlg_platform_h);

    _labPlatform = new QLabel(pView);
    _labUrl = new QLabel(pView);
    _labDisplay = new QLabel(pView);

    m_labHint = new QLabel(pView);
    m_labHint->setAlignment(Qt::AlignCenter);

    m_editPlatform = new QLineEdit(pView);
    m_editUrl = new QLineEdit(pView);
    m_editDisplay = new QLineEdit(pView);

    m_btnOK = new QPushButton(pView);
    m_btnCancel = new QPushButton(pView);

    QHBoxLayout* phLayout1 = new QHBoxLayout;
    phLayout1->setMargin(0);
    phLayout1->setSpacing(2);
    phLayout1->addWidget(_labPlatform);
    phLayout1->addWidget(m_editPlatform);

    QHBoxLayout* phLayout2 = new QHBoxLayout;
    phLayout2->setMargin(0);
    phLayout2->setSpacing(2);
    phLayout2->addWidget(_labUrl);
    phLayout2->addWidget(m_editUrl);

    QHBoxLayout* phLayout3 = new QHBoxLayout;
    phLayout3->setMargin(0);
    phLayout3->setSpacing(2);
    phLayout3->addWidget(_labDisplay);
    phLayout3->addWidget(m_editDisplay);

    QHBoxLayout* phLayout4 = new QHBoxLayout;
    phLayout4->addStretch(1);
    phLayout4->addWidget(m_btnOK);
    phLayout4->addStretch(1);
    phLayout4->addWidget(m_btnCancel);
    phLayout4->addStretch(1);

    QVBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->setContentsMargins(4, 4, 4, 4);
    pMainLayout->setSpacing(4);
    pMainLayout->addWidget(m_labHint);
    pMainLayout->addLayout(phLayout1);
    pMainLayout->addLayout(phLayout2);
    pMainLayout->addLayout(phLayout3);
    pMainLayout->addLayout(phLayout4);

    pView->setLayout(pMainLayout);

    RetranslateUI(pView);
}

void EditPlatformDialog::ui_type::RetranslateUI(EditPlatformDialog * pView)
{
    _labPlatform->setText("Platform:");
    _labUrl->setText("Url:");
    _labDisplay->setText("Display:");

    m_btnOK->setText("OK");
    m_btnCancel->setText("Cancel");
}

//==============================================================================
// Implementation of EditAccountDialog
//==============================================================================
EditAccountDialog::EditAccountDialog(bnb::platform_type & platform, bnb::account_type & account, delegate_type * ptrDelegate, QWidget * parent)
    : QDialog(parent)
    , m_ptrDelegate(ptrDelegate)
    , m_Account(account)
    , m_Platform(platform)
{
    _ui.SetupUI(this);

    _ui.m_editPlatform->setText(QString::fromStdString(m_Platform.m_Key));

    if (!m_Account.m_Key.empty())
        _ui.m_editAccount->setText(QString::fromStdString(m_Account.m_Key));

    if (!m_Account.m_Value.m_strDisplay.empty())
        _ui.m_editDisplay->setText(QString::fromStdString(m_Account.m_Value.m_strDisplay));

    QObject::connect(_ui.m_editAccount, &QLineEdit::textEdited, this, &EditAccountDialog::OnChangedText);
    QObject::connect(_ui.m_editDisplay, &QLineEdit::textEdited, this, &EditAccountDialog::OnChangedText);
    QObject::connect(_ui.m_btnOK, &QPushButton::clicked, this, &EditAccountDialog::OnClickedOK);
    QObject::connect(_ui.m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

void EditAccountDialog::OnChangedText(const QString &)
{
    _ui.m_labHint->clear();
}

void EditAccountDialog::OnClickedOK()
{
    if (_ui.m_editAccount->text().isEmpty())
    {
        _ui.m_labHint->setText("Account name mustn\'t be null !");
        return;
    }

    m_Account.m_Key = _ui.m_editAccount->text().toStdString();
    m_Account.m_Value.m_strDisplay = _ui.m_editDisplay->text().toStdString();

    if (m_ptrDelegate->CanUpdate(m_Platform, m_Account))
    {
        accept();
        return;
    }

    _ui.m_labHint->setText("The account name you entered already exists or is invalid !");
}

//------------------------------------------------------------------------------

void EditAccountDialog::ui_type::SetupUI(EditAccountDialog * pView)
{
    pView->setObjectName("EditPasswordDialog");
    pView->setFixedSize(ui_utils::dlg_password_w, ui_utils::dlg_password_h);

    _labPlatform = new QLabel(pView);
    _labAccount = new QLabel(pView);
    _labDisplay = new QLabel(pView);

    m_labHint = new QLabel(pView);
    m_labHint->setAlignment(Qt::AlignCenter);

    m_editPlatform = new QLineEdit(pView);
    m_editPlatform->setReadOnly(true);
    m_editAccount = new QLineEdit(pView);
    m_editDisplay = new QLineEdit(pView);

    m_btnOK = new QPushButton(pView);
    m_btnCancel = new QPushButton(pView);

    QHBoxLayout* phLayout1 = new QHBoxLayout;
    phLayout1->setMargin(0);
    phLayout1->setSpacing(2);
    phLayout1->addWidget(_labPlatform);
    phLayout1->addWidget(m_editPlatform);

    QHBoxLayout* phLayout2 = new QHBoxLayout;
    phLayout2->setMargin(0);
    phLayout2->setSpacing(2);
    phLayout2->addWidget(_labAccount);
    phLayout2->addWidget(m_editAccount);

    QHBoxLayout* phLayout3 = new QHBoxLayout;
    phLayout3->setMargin(0);
    phLayout3->setSpacing(2);
    phLayout3->addWidget(_labDisplay);
    phLayout3->addWidget(m_editDisplay);

    QHBoxLayout* phLayout4 = new QHBoxLayout;
    phLayout4->addStretch(1);
    phLayout4->addWidget(m_btnOK);
    phLayout4->addStretch(1);
    phLayout4->addWidget(m_btnCancel);
    phLayout4->addStretch(1);

    QVBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->setContentsMargins(4, 4, 4, 4);
    pMainLayout->setSpacing(4);
    pMainLayout->addWidget(m_labHint);
    pMainLayout->addLayout(phLayout1);
    pMainLayout->addLayout(phLayout2);
    pMainLayout->addLayout(phLayout3);
    pMainLayout->addLayout(phLayout4);

    pView->setLayout(pMainLayout);

    RetranslateUI(pView);
}

void EditAccountDialog::ui_type::RetranslateUI(EditAccountDialog * pView)
{
    _labPlatform->setText("Platform:");
    _labAccount->setText("Account:");
    _labDisplay->setText("Display:");

    m_btnOK->setText("OK");
    m_btnCancel->setText("Cancel");
}
