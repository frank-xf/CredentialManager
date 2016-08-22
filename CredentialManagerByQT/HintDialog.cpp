
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>

#include "HintDialog.h"

HintDialog::HintDialog(const QString& strText, const QString& strTitle, QWidget * parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
{
    _ui.SetupUI(this);

    SetText(strText);
    setWindowTitle(strTitle);

    QObject::connect(_ui.m_btnOK, &QPushButton::clicked, this, &QDialog::accept);
}

HintDialog::~HintDialog() {
	
}

void HintDialog::SetText(const QString & strText)
{
    _ui.m_labText->setText(strText);
}

QString HintDialog::GetText() const
{
    return _ui.m_labText->text();
}

void HintDialog::ui_type::SetupUI(HintDialog * pDlg)
{
    m_labText = new QLabel(pDlg);
    m_btnOK = new QPushButton(pDlg);

    QVBoxLayout* pLayout = new QVBoxLayout;
    pLayout->addWidget(m_labText);
    pLayout->addWidget(m_btnOK);

    pDlg->setLayout(pLayout);

    RetranslateUI(pDlg);
}

void HintDialog::ui_type::RetranslateUI(HintDialog * pDlg)
{
    m_btnOK->setText("OK");
}
