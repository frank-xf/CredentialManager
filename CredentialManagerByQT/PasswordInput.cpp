
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>

#include "PasswordInput.h"

PasswordInput::PasswordInput(QWidget * parent) : QDialog(parent)
{
    _ui.SetupUI(this);

    QObject::connect(_ui.m_btnOK, &QPushButton::clicked, this, &QDialog::accept);
    QObject::connect(_ui.m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

QString PasswordInput::GetPassword() const
{
    return _ui.m_editPassword->text();
}

void PasswordInput::ui_type::SetupUI(PasswordInput * pDlg)
{
    m_editPassword = new QLineEdit(pDlg);
    m_editPassword->setEchoMode(QLineEdit::Password);

    m_btnOK = new QPushButton(pDlg);
    m_btnCancel = new QPushButton(pDlg);

    QHBoxLayout* phLayout1 = new QHBoxLayout;
    phLayout1->addStretch(1);
    phLayout1->addWidget(m_editPassword);
    phLayout1->addStretch(1);

    QHBoxLayout* phLayout2 = new QHBoxLayout;
    phLayout2->addStretch(1);
    phLayout2->addWidget(m_btnOK);
    phLayout2->addStretch(1);
    phLayout2->addWidget(m_btnCancel);
    phLayout2->addStretch(1);

    QVBoxLayout* pvLayout = new QVBoxLayout;
    pvLayout->addStretch(1);
    pvLayout->addLayout(phLayout1);
    pvLayout->addStretch(1);
    pvLayout->addLayout(phLayout2);
    pvLayout->addStretch(1);

    pDlg->setLayout(pvLayout);

    RetranslateUI(pDlg);
}

void PasswordInput::ui_type::RetranslateUI(PasswordInput * pDlg)
{
    m_btnOK->setText("OK");
    m_btnCancel->setText("Cancel");
}
