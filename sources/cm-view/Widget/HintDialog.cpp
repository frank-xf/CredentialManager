#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>

#include "credential_qt_utils.h"

#include "Widget/HintDialog.h"

QT_BEGIN_NAMESPACE

template<typename _Ty>
inline constexpr unsigned int _to_uint(_Ty value) { return static_cast<unsigned int>(value); }

static const QColor _g_clrText[_to_uint(hint_type::ht_max)]{
    { 0xFF, 0x00, 0x00 },{ 0xC0, 0xC0, 0x00 },{ 0x00, 0x80, 0x00 }
};

static const QString _g_strTitle[_to_uint(hint_type::ht_max)]{
    "error", "warning", "info"
};

static const QString _g_qrcIcon[_to_uint(hint_type::ht_max)]{
    ":/CredentialManager/Resources/image/error.png",
    ":/CredentialManager/Resources/image/warning.png",
    ":/CredentialManager/Resources/image/info.png"
};

HintDialog::HintDialog(hint_type hType, const QString& strText, QWidget * parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
{
    _ui.SetupUI(this);

    _ui.m_labText->setText(strText);
    _ui.m_labText->setStyleSheet(
        "QLabel{ background:transparent; color:" + _g_clrText[static_cast<unsigned int>(hType)].name() + "; }");

    setWindowTitle(_g_strTitle[static_cast<unsigned int>(hType)]);
    setWindowIcon(QIcon(_g_qrcIcon[static_cast<unsigned int>(hType)]));
    setFixedSize(sizeHint());

    QObject::connect(_ui.m_btnClose, &QPushButton::clicked, this, &QDialog::accept);
}

void HintDialog::ui_type::SetupUI(HintDialog * pDlg)
{
    pDlg->setObjectName("HintDialog");

    ui_utils::SetBackgroundColor(pDlg, Qt::white);

    m_labText = new QLabel(pDlg);
    m_labText->setWordWrap(true);
    m_labText->setAlignment(Qt::AlignCenter);
    m_labText->setMinimumWidth(ui_utils::edit_default_w);
    m_labText->setFont(ui_utils::MakeFont());

    m_btnClose = ui_utils::MakeButton(pDlg);

    QHBoxLayout* phLayout = new QHBoxLayout;
    phLayout->setMargin(0);
    phLayout->setSpacing(0);
    phLayout->addStretch(1);
    phLayout->addWidget(m_btnClose);
    phLayout->addStretch(1);

    QVBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->setContentsMargins(4, 8, 4, 10);
    pMainLayout->setSpacing(20);
    pMainLayout->addWidget(m_labText, 1);
    pMainLayout->addLayout(phLayout);

    pDlg->setLayout(pMainLayout);

    RetranslateUI(pDlg);
}

void HintDialog::ui_type::RetranslateUI(HintDialog * pDlg)
{
    m_btnClose->setText("Close");
}

ConfirmDialog::ConfirmDialog(const QString & strText, QWidget * parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
{
    _ui.SetupUI(this);

    _ui.m_labText->setText(strText);

    setFixedSize(sizeHint());

    QObject::connect(_ui.m_btnOK, &QPushButton::clicked, this, &QDialog::accept);
    QObject::connect(_ui.m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

void ConfirmDialog::ui_type::SetupUI(ConfirmDialog * pDlg)
{
    pDlg->setObjectName("ConfirmDialog");
    pDlg->setWindowTitle("Confirm");
    pDlg->setWindowIcon(QIcon(_g_qrcIcon[_to_uint(hint_type::ht_info)]));

    ui_utils::SetBackgroundColor(pDlg, Qt::white);

    m_labText = new QLabel(pDlg);
    m_labText->setMinimumWidth(ui_utils::edit_default_w);
    m_labText->setAlignment(Qt::AlignCenter);
    m_labText->setFont(ui_utils::MakeFont());
    m_labText->setStyleSheet("QLabel{ background:transparent; color:#008000; }");

    m_btnOK = ui_utils::MakeButton(pDlg);
    m_btnCancel = ui_utils::MakeButton(pDlg);

    m_btnOK->setDefault(true);

    QHBoxLayout* phLayout = ui_utils::MakeButtonLayout(m_btnOK, m_btnCancel);

    QVBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->setContentsMargins(8, 8, 2, 8);
    pMainLayout->setSpacing(4);
    pMainLayout->addWidget(m_labText, 1);
    pMainLayout->addLayout(phLayout);

    pDlg->setLayout(pMainLayout);

    RetranslateUI(pDlg);
}

void ConfirmDialog::ui_type::RetranslateUI(ConfirmDialog * pDlg)
{
    m_btnOK->setText("OK");
    m_btnCancel->setText("Cancel");
}

QT_END_NAMESPACE
