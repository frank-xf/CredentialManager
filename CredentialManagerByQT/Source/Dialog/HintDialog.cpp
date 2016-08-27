#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>

#include "Dialog/HintDialog.h"

QT_BEGIN_NAMESPACE

static const QColor _g_colorText[static_cast<unsigned int>(hint_type::ht_max)]{
    QColor(0xff, 0x00, 0x00), QColor(0x80, 0x80, 0x00), QColor(0x00, 0x80, 0x00)
};

static const QString _g_strTitle[static_cast<unsigned int>(hint_type::ht_max)]{
    "error", "warning", "info"
};

HintDialog::HintDialog(hint_type hType, const QString& strText, QWidget * parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
{
    _ui.SetupUI(this);

    setWindowTitle(_g_strTitle[static_cast<unsigned int>(hType)]);

    SetText(hType, strText);

    QObject::connect(_ui.m_btnClose, &QPushButton::clicked, this, &QDialog::accept);
}

void HintDialog::SetText(hint_type hType, const QString & strText)
{
    _ui.m_labText->setText(strText);

    setFixedSize(sizeHint());
}

QString HintDialog::GetText() const
{
    return _ui.m_labText->text();
}

void HintDialog::ui_type::SetupUI(HintDialog * pDlg)
{
    m_labText = new QLabel(pDlg);
    m_labText->setAlignment(Qt::AlignCenter);
    m_btnClose = new QPushButton(pDlg);

    QHBoxLayout* phLayout = new QHBoxLayout;
    phLayout->setMargin(4);
    phLayout->addStretch(1);
    phLayout->addWidget(m_btnClose);
    phLayout->addStretch(1);

    QVBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->setContentsMargins(0, 0, 0, 4);
    pMainLayout->setSpacing(2);
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
