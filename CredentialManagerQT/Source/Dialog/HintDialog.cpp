#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>

#include "credential_qt_utils.h"

#include "Dialog/HintDialog.h"

QT_BEGIN_NAMESPACE

static const QColor _g_clrText[ui_utils::to_uint(hint_type::ht_max)]{
    QColor(0xFF, 0x00, 0x00), QColor(0xC0, 0xC0, 0x00), QColor(0x00, 0x80, 0x00)
};

static const QString _g_strTitle[ui_utils::to_uint(hint_type::ht_max)]{
    "error", "warning", "info"
};

static const QString _g_qrcIcon[ui_utils::to_uint(hint_type::ht_max)]{
    ":/CredentialManager/Resources/image/error.png",
    ":/CredentialManager/Resources/image/warning.png",
    ":/CredentialManager/Resources/image/info.png"
};

HintDialog::HintDialog(hint_type hType, const QString& strText, QWidget * parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
{
    ui_utils::SetBackgroundColor(this, Qt::white);

    _ui.SetupUI(this);

    setWindowTitle(_g_strTitle[static_cast<unsigned int>(hType)]);
    setWindowIcon(QIcon(_g_qrcIcon[static_cast<unsigned int>(hType)]));

    SetText(hType, strText);

    QObject::connect(_ui.m_btnClose, &QPushButton::clicked, this, &QDialog::accept);
}

void HintDialog::SetText(hint_type hType, const QString & strText)
{
    _ui.m_labText->setText(strText);

    QPalette palette;
    palette.setColor(QPalette::WindowText, _g_clrText[static_cast<unsigned int>(hType)]);
    _ui.m_labText->setPalette(palette);

    setFixedSize(sizeHint());
}

QString HintDialog::GetText() const
{
    return _ui.m_labText->text();
}

void HintDialog::ui_type::SetupUI(HintDialog * pDlg)
{
    pDlg->setObjectName("HintDialog");

    m_labText = new QLabel(pDlg);
    m_labText->setWordWrap(true);
    m_labText->setAlignment(Qt::AlignCenter);
    m_labText->setFixedWidth(320);
    m_labText->setStyleSheet("QLabel{ background:transparent; }");

    m_btnClose = new QPushButton(pDlg);

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

QT_END_NAMESPACE
