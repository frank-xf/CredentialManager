#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QLabel>

#include "HintDialog.h"
#include "PasswordInput.h"
#include "CreateDialog.h"
#include "CredentialMainView.h"

QT_BEGIN_NAMESPACE

CredentialMainView::CredentialMainView(QWidget *parent)
    : QWidget(parent)
{
    _ui.SetupUI(this);

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

    dlg.exec();
}

void CredentialMainView::OnClickedOpen()
{
    /*
    QString strFile = QFileDialog::getOpenFileName(
        this, "Please select a credential file", ".", "credential file(*.credential)");
        */

    QString strFile("def.credential");
    if (strFile.isEmpty()) return;
    
    bnb::memory_type dst;

    switch (bnb::Credential::CheckFile(strFile.toUtf8(), &dst))
    {
    case bnb::result_type::rt_file_error:
        HintDialog("You selected file error !", "error", this).exec();
        return;
    case bnb::result_type::rt_file_invalid:
        HintDialog("You selected file invalid !", "error", this).exec();
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
            HintDialog("You input password invalid !", "error", this).exec();
            return;
        case bnb::result_type::rt_password_error:
            HintDialog("You input password error !", "error", this).exec();
            return;
        case bnb::result_type::rt_file_error:
            HintDialog("Anaylze file failed !", "error", this).exec();
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

    UpdateCredentail(new CredentialView(m_Credential, this, this));
}

void CredentialMainView::OnClickedMotifyName()
{
}

void CredentialMainView::OnClickedMotifyWord()
{
}

void CredentialMainView::UpdateCredentail(CredentialView * view)
{
    QWidget* old = _ui.m_areaCredential->takeWidget();
    if (nullptr != old) delete old;

    _ui.m_areaCredential->setWidget(view);
    _ui.m_areaCredential->setMaximumSize(view->width() + 20, view->height() + 2);

    setFixedWidth(_ui.m_areaCredential->maximumWidth() + 8);
    setMaximumHeight(_ui.m_areaCredential->maximumHeight() + 100);
}

void CredentialMainView::ui_type::SetupUI(CredentialMainView* pView)
{
    m_btnNew = new QPushButton(pView);
    m_btnOpen = new QPushButton(pView);
    m_btnMotifyName = new QPushButton(pView);
    m_btnMotifyWord = new QPushButton(pView);

    QHBoxLayout* phLayout1 = new QHBoxLayout;
    phLayout1->setSpacing(8);
    phLayout1->setMargin(0);
    phLayout1->addWidget(m_btnNew);
    phLayout1->addWidget(m_btnOpen);
    phLayout1->addStretch(1);
    phLayout1->addWidget(m_btnMotifyName);
    phLayout1->addWidget(m_btnMotifyWord);

    m_areaCredential = new QScrollArea(pView);
    m_areaCredential->setWidget(new QLabel("Please select credential file"));
    m_areaCredential->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_areaCredential->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_labFile = new QLabel(pView);
    m_labTime = new QLabel(pView);
    m_barTime = new QProgressBar(pView);

    QVBoxLayout* pvLayout1 = new QVBoxLayout;
    pvLayout1->setSpacing(8);
    pvLayout1->setMargin(0);
    pvLayout1->addWidget(m_labTime);
    pvLayout1->addWidget(m_barTime);

    QHBoxLayout* phLayout2 = new QHBoxLayout;
    phLayout2->setSpacing(8);
    phLayout2->setMargin(0);
    phLayout2->addWidget(m_labFile, 1);
    phLayout2->addLayout(pvLayout1);

    QVBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->setSpacing(4);
    pMainLayout->setMargin(4);

    pMainLayout->addLayout(phLayout1);
    pMainLayout->addWidget(m_areaCredential, 1);
    pMainLayout->addLayout(phLayout2);

    pView->setLayout(pMainLayout);

    RetranslateUI(pView);
}

void CredentialMainView::ui_type::RetranslateUI(CredentialMainView * pView)
{
    m_btnNew->setText("New");
    m_btnOpen->setText("Open");
    m_btnMotifyName->setText("Motify User");
    m_btnMotifyWord->setText("Motify Password");
}

QT_END_NAMESPACE

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
    return false;
}
