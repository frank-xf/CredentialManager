#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

#include "credential_qt_utils.h"

#include "Widget/PasswordInput.h"

QT_BEGIN_NAMESPACE

PasswordInput::PasswordInput(QWidget * parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
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
    pDlg->setObjectName("PasswordInput");
    ui_utils::SetBackgroundColor(pDlg, Qt::white);

    _labPassword = ui_utils::MakeStaticLabel(pDlg);

    m_editPassword = ui_utils::MakePasswordLine(pDlg);

    m_btnOK = ui_utils::MakeButton(pDlg);
    m_btnCancel = ui_utils::MakeButton(pDlg);

    m_editPassword->setFocus();
    m_btnOK->setDefault(true);

    QHBoxLayout* phLayout1 = new QHBoxLayout;
    phLayout1->setMargin(0);
    phLayout1->setSpacing(0);
    phLayout1->addWidget(_labPassword);
    phLayout1->addWidget(m_editPassword);
    phLayout1->addWidget(ui_utils::MakeMarkLabel(pDlg));

    QHBoxLayout* phLayout2 = ui_utils::MakeButtonLayout(m_btnOK, m_btnCancel);

    QVBoxLayout* pvMainLayout = new QVBoxLayout;
    pvMainLayout->setContentsMargins(4, 8, 4, 4);
    pvMainLayout->setSpacing(4);
    pvMainLayout->addLayout(phLayout1);
    pvMainLayout->addLayout(phLayout2);

    RetranslateUI(pDlg);

    pDlg->setLayout(pvMainLayout);
    pDlg->setFixedSize(pDlg->sizeHint());
}

void PasswordInput::ui_type::RetranslateUI(PasswordInput * pDlg)
{
    _labPassword->setText("Password: ");

    m_btnOK->setText("OK");
    m_btnCancel->setText("Cancel");
}

QT_END_NAMESPACE
