#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QLabel>

#include "Major/CredentialMainView.h"
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
    _ui.SetupUI(this);

    UpdateSize();

    QObject::connect(_ui.m_btnNew, &QPushButton::clicked, this, &CredentialMainView::OnClickedNew);
    QObject::connect(_ui.m_btnOpen, &QPushButton::clicked, this, &CredentialMainView::OnClickedOpen);
    QObject::connect(_ui.m_btnMotifyName, &QPushButton::clicked, this, &CredentialMainView::OnClickedMotifyName);
    QObject::connect(_ui.m_btnMotifyWord, &QPushButton::clicked, this, &CredentialMainView::OnClickedMotifyWord);
}

CredentialMainView::~CredentialMainView()
{

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
    // if (QDialog::Accepted == dlg.exec())
    {
        QString password = "123"; // dlg.GetPassword();
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
}

void CredentialMainView::OnClickedMotifyName()
{
}

void CredentialMainView::OnClickedMotifyWord()
{
}

void CredentialMainView::UpdateCredentail()
{
    CredentialView * view = new CredentialView(m_Credential, this, this);

    QWidget* old = _ui.m_areaCredential->takeWidget();
    if (nullptr != old) delete old;

    _ui.m_areaCredential->setWidget(view);
    _ui.m_areaCredential->setMaximumSize(view->width() + 20, view->height() + 2);

    UpdateSize();

    if (!_ui.m_btnMotifyName->isEnabled()) _ui.m_btnMotifyName->setEnabled(true);
    if (!_ui.m_btnMotifyWord->isEnabled()) _ui.m_btnMotifyWord->setEnabled(true);

    setWindowTitle(QString::fromStdString(m_Credential.GetUser()) + " - " + m_strFile);
}

void CredentialMainView::UpdateSize()
{
    setFixedWidth(_ui.m_areaCredential->maximumWidth() + 8);
    setMaximumHeight(_ui.m_areaCredential->maximumHeight() + 32);
}

void CredentialMainView::ui_type::SetupUI(CredentialMainView* pView)
{
    pView->setObjectName("CredentialMainView");
    pView->setWindowTitle("Credential Manager");

    m_btnNew = new QPushButton(pView);
    m_btnNew->setFixedHeight(20);
    m_btnOpen = new QPushButton(pView);
    m_btnOpen->setFixedHeight(20);
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

bool CredentialMainView::OnAddPlatform()
{
    return false;
}

bool CredentialMainView::OnAddAccount()
{
    return false;
}

bool CredentialMainView::OnRemovePlatform(bnb::platform_type * pp)
{
    return false;
}

bool CredentialMainView::OnRemoveAccount(bnb::platform_type * pp, bnb::account_type * pa)
{
    return false;
}

bool CredentialMainView::OnEditPlatform(bnb::platform_type * pp)
{
    return false;
}

bool CredentialMainView::OnEditAccount(bnb::platform_type * pp, bnb::account_type * pa)
{
    return false;
}

bool CredentialMainView::OnViewCredential(bnb::platform_type * pp, bnb::account_type * pa)
{
    CredentialDialog dlg(m_Credential, this);

    dlg.exec();

    return true;
}

bool CredentialMainView::CanUpdate(const bnb::platform_type & platform)
{
    return m_Credential.List().CanUpdate(platform);
}

bool CredentialMainView::CanUpdate(const bnb::platform_type & platform, const bnb::account_type & account)
{
    auto ptr_platform = m_Credential.List().Find(platform);
    if (ptr_platform)
        return ptr_platform->m_Value.CanUpdate(account);

    return false;
}

QT_END_NAMESPACE
