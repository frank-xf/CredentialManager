#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

#include "../cm_view_utils.h"

#include "LoginDialog.h"

QT_BEGIN_NAMESPACE

LoginDialog::LoginDialog(QWidget * parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
{
    _ui.SetupUI(this);

    QObject::connect(_ui.m_btnOK, &QPushButton::clicked, this, &QDialog::accept);
    QObject::connect(_ui.m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

QString LoginDialog::GetPassword() const
{
    return _ui.m_editPassword->text();
}

QString LoginDialog::GetUsername() const
{
    return _ui.m_editUsername->text();
}

void LoginDialog::ui_type::SetupUI(LoginDialog * pDlg)
{
    pDlg->setObjectName("LoginDialog");
    SetBackgroundColor(pDlg, Qt::white);

    _labPassword = MakeStaticLabel(pDlg);

    m_editPassword = MakePasswordLine(pDlg);

    m_btnOK = MakeButton(pDlg);
    m_btnCancel = MakeButton(pDlg);

    m_editPassword->setFocus();
    m_btnOK->setDefault(true);

    QHBoxLayout* phLayout1 = new QHBoxLayout;
    phLayout1->setMargin(0);
    phLayout1->setSpacing(0);
    phLayout1->addWidget(_labPassword);
    phLayout1->addWidget(m_editPassword);
    phLayout1->addWidget(MakeMarkLabel(pDlg));

    QHBoxLayout* phLayout2 = MakeButtonLayout(m_btnOK, m_btnCancel);

    QVBoxLayout* pvMainLayout = new QVBoxLayout;
    pvMainLayout->setContentsMargins(4, 8, 4, 4);
    pvMainLayout->setSpacing(4);
    pvMainLayout->addLayout(phLayout1);
    pvMainLayout->addLayout(phLayout2);

    RetranslateUI(pDlg);

    pDlg->setLayout(pvMainLayout);
    pDlg->setFixedSize(pDlg->sizeHint());
}

void LoginDialog::ui_type::RetranslateUI(LoginDialog * pDlg)
{
    _labUsername->setText("Username: ");
    _labPassword->setText("Password: ");

    m_btnOK->setText("OK");
    m_btnCancel->setText("Cancel");
}

QT_END_NAMESPACE
