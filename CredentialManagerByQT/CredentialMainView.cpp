#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QLabel>

#include "CredentialMainView.h"
#include "CredentialView.h"
#include "PasswordInput.h"

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
}

void CredentialMainView::OnClickedOpen()
{
    QString strFile = QFileDialog::getOpenFileName(
        this, "Please select a credential file", ".", "credential file(*.credential)");

    if (strFile.isEmpty()) return;
    
    bnb::memory_type dst;

    switch (bnb::Credential::CheckFile(strFile.toUtf8(), &dst))
    {
    case bnb::result_type::rt_file_error:
        return;
    case bnb::result_type::rt_file_invalid:
        return;
    default:
        break;
    }

    PasswordInput dlg(this);
    if (QDialog::Accepted == dlg.exec())
    {
        QString password = dlg.GetPassword();
        switch (bnb::Credential::Decoding(dst, (const unsigned char*)password.toStdString().c_str(), password.size()))
        {
        case bnb::result_type::rt_password_invalid:
            return;
        case bnb::result_type::rt_password_error:
            return;
        case bnb::result_type::rt_file_error:
            return;
        default:
            break;
        }
    }

    if (!m_Credential.FromXml(dst))
    {

        return;
    }

    QWidget* old = _ui.m_areaCredential->takeWidget();
    _ui.m_areaCredential->setWidget(new CredentialView(nullptr, m_Credential));
    if (nullptr != old) delete old;
}

void CredentialMainView::OnClickedMotifyName()
{
}

void CredentialMainView::OnClickedMotifyWord()
{
}

void CredentialMainView::ui_type::SetupUI(CredentialMainView* pView)
{
    m_btnNew = new QPushButton(pView);
    m_btnOpen = new QPushButton(pView);
    m_btnMotifyName = new QPushButton(pView);
    m_btnMotifyWord = new QPushButton(pView);

    QHBoxLayout* phLayout1 = new QHBoxLayout;
    phLayout1->addWidget(m_btnNew);
    phLayout1->addWidget(m_btnOpen);
    phLayout1->addWidget(m_btnMotifyName);
    phLayout1->addWidget(m_btnMotifyWord);
    phLayout1->addStretch(1);

    m_areaCredential = new QScrollArea(pView);
    m_areaCredential->setWidget(new QLabel("Please select credential file"));

    m_labFile = new QLabel(pView);
    m_labTime = new QLabel(pView);
    m_barTime = new QProgressBar(pView);

    QVBoxLayout* pvLayout1 = new QVBoxLayout;
    pvLayout1->addWidget(m_labTime);
    pvLayout1->addWidget(m_barTime);

    QHBoxLayout* phLayout2 = new QHBoxLayout;
    phLayout2->addWidget(m_labFile, 1);
    phLayout2->addLayout(pvLayout1);

    QVBoxLayout* pMainLayout = new QVBoxLayout;
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