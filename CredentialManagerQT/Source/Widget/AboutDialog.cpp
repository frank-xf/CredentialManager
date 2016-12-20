#include <QtCore/QFile>
#include <QtGui/QTextBlock>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QDialog>
#include <QtWidgets/QTextEdit>

#include "credential_qt_utils.h"

#include "Widget/AboutDialog.h"

QT_BEGIN_NAMESPACE

AboutDialog::AboutDialog(QWidget * parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
{
    _ui.SetupUI(this);

    QFile file(":/CredentialManager/Resources/text/description.txt");
    file.open(QIODevice::ReadOnly);
    _ui.m_editText->setText(file.readAll());
    file.close();

    for (auto textBlock = _ui.m_editText->document()->begin(); textBlock.isValid(); textBlock = textBlock.next())
    {
        QTextCursor textCursor = QTextCursor(textBlock);
        QTextBlockFormat blockFormat = textBlock.blockFormat();

        blockFormat.setTopMargin(8);
        blockFormat.setBottomMargin(8);
        textCursor.setBlockFormat(blockFormat);
    }

    _ui.m_editText->document()->adjustSize();
    int w(_ui.m_editText->document()->size().width());
    int h(_ui.m_editText->document()->size().height());
    _ui.m_editText->setMinimumSize(w + 32, h + 32);

    setFixedSize(sizeHint());

    QObject::connect(_ui.m_btnClose, &QPushButton::clicked, this, &QDialog::accept);
}

void AboutDialog::ui_type::SetupUI(AboutDialog * pDlg)
{
    pDlg->setObjectName("ConfirmDialog");
    pDlg->setWindowTitle("About");
    ui_utils::SetBackgroundColor(pDlg, Qt::white);

    m_editText = new QTextEdit(pDlg);
    m_editText->setReadOnly(true);
    m_editText->setTextColor(ui_utils::g_clrPair);
    m_editText->setFontPointSize(ui_utils::def_text_size);
    m_editText->setLineWrapMode(QTextEdit::NoWrap);
    m_editText->setFrameStyle(QFrame::NoFrame);
    m_editText->document()->setDocumentMargin(0);

    m_btnClose = ui_utils::MakeButton(pDlg);

    QHBoxLayout* phLayout = new QHBoxLayout;
    phLayout->setContentsMargins(0, 4, 0, 8);
    phLayout->setSpacing(0);
    phLayout->addStretch(1);
    phLayout->addWidget(m_btnClose);
    phLayout->addStretch(1);

    QVBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->setContentsMargins(4, 16, 4, 4);
    pMainLayout->setSpacing(0);
    pMainLayout->addWidget(m_editText, 1);
    pMainLayout->addLayout(phLayout);

    pDlg->setLayout(pMainLayout);

    RetranslateUI(pDlg);
}

void AboutDialog::ui_type::RetranslateUI(AboutDialog * pDlg)
{
    m_btnClose->setText("Close");
}

QT_END_NAMESPACE
