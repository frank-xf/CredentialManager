
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>

#include "credential_qt_manager.h"
#include "credential_model_manager.h"

#include "Dialog/CreateDialog.h"

QT_BEGIN_NAMESPACE

CreateDialog::CreateDialog(QWidget * parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
{
    _ui.SetupUI(this);

    setFixedSize(420, 168);

    QObject::connect(_ui.m_btnBrowse, &QPushButton::clicked, this, &CreateDialog::OnClickedBrowse);
    QObject::connect(_ui.m_btnOK, &QPushButton::clicked, this, &CreateDialog::OnClickedOK);
    QObject::connect(_ui.m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

QString CreateDialog::GetUserName() const
{
    return _ui.m_editUserName->text();
}

QString CreateDialog::GetPassword() const
{
    return _ui.m_editPassword->text();
}

QString CreateDialog::GetFilePath() const
{
    return _ui.m_editFilePath->text();
}

void CreateDialog::OnClickedOK()
{
    if (_ui.m_editUserName->text().isEmpty())
    {
        _ui.m_labHint->setText("User Name mustn\'t be null !");
        return;
    }

    QString s1 = _ui.m_editPassword->text();
    if (s1.isEmpty())
    {
        _ui.m_labHint->setText("Password mustn\'t be null !");
        return;
    }

    QString s2 = _ui.m_editValidate->text();
    if (s1 != s2)
    {
        _ui.m_labHint->setText("The two passwords you entered did not match !");
        return;
    }

    if (_ui.m_editFilePath->text().isEmpty())
    {
        _ui.m_labHint->setText("Credential file path mustn\'t be null !");
        return;
    }

	g_AppMgr.Model().Info().Clear();
	g_AppMgr.Model().Info().SetUser(_ui.m_editUserName->text().toStdString());
	g_AppMgr.Model().Info().SetWord(s1.toStdString());
	g_AppMgr.Model().SetFile(_ui.m_editFilePath->text().toStdString());
	g_AppMgr.Model().SaveCredential();

    accept();
}

void CreateDialog::OnClickedBrowse()
{
    QString strText;
    if (_ui.m_editUserName->text().isEmpty())
        strText = "nemo";
    else
        strText = _ui.m_editUserName->text();

    QString strFile = QFileDialog::getSaveFileName(
        this, "Please input a credential file", strText, "credential file(*.credential)");

    if (!strFile.isEmpty())
    {
        _ui.m_editFilePath->setText(strFile);
    }
}

void CreateDialog::ui_type::SetupUI(CreateDialog * pView)
{
    _labUserName = new QLabel(pView);
    _labUserName->setFixedSize(60, 20);
    _labUserName->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    _labPassword = new QLabel(pView);
    _labPassword->setFixedSize(60, 20);
    _labPassword->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    _labValidate = new QLabel(pView);
    _labValidate->setFixedSize(60, 20);
    _labValidate->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    m_labHint = new QLabel(pView);
    m_labHint->setAlignment(Qt::AlignCenter);

    m_btnOK = new QPushButton(pView);
    m_btnCancel = new QPushButton(pView);
    m_btnBrowse = new QPushButton(pView);
    m_btnBrowse->setFixedSize(60, 20);
    m_btnBrowse->setStyleSheet(
        "QPushButton{ background-color:transparent; border:none; color:#0000FF; text-align:right; }\n"
        "QPushButton:hover{ color:#FF0000; }\n"
        "QPushButton:pressed{ color:#008000; }");

    m_editUserName = new QLineEdit(pView);
    m_editUserName->setFixedSize(200, 20);
    m_editUserName->setMaxLength(64);
    m_editPassword = new QLineEdit(pView);
    m_editPassword->setFixedSize(200, 20);
    m_editPassword->setMaxLength(64);
    m_editPassword->setEchoMode(QLineEdit::Password);
    m_editValidate = new QLineEdit(pView);
    m_editValidate->setFixedSize(200, 20);
    m_editValidate->setMaxLength(64);
    m_editValidate->setEchoMode(QLineEdit::Password);
    m_editFilePath = new QLineEdit(pView);
    m_editFilePath->setFixedSize(320, 20);
    m_editFilePath->setReadOnly(true);

    QHBoxLayout* phLayout1 = new QHBoxLayout;
    phLayout1->setSpacing(2);
    phLayout1->setMargin(0);
    phLayout1->addWidget(_labUserName);
    phLayout1->addWidget(m_editUserName);
    phLayout1->addStretch(1);

    QHBoxLayout* phLayout2 = new QHBoxLayout;
    phLayout2->setSpacing(2);
    phLayout2->setMargin(0);
    phLayout2->addWidget(_labPassword);
    phLayout2->addWidget(m_editPassword);
    phLayout2->addStretch(1);

    QHBoxLayout* phLayout3 = new QHBoxLayout;
    phLayout3->setSpacing(2);
    phLayout3->setMargin(0);
    phLayout3->addWidget(_labValidate);
    phLayout3->addWidget(m_editValidate);
    phLayout3->addStretch(1);

    QHBoxLayout* phLayout4 = new QHBoxLayout;
    phLayout4->setSpacing(2);
    phLayout4->setMargin(0);
    phLayout4->addWidget(m_btnBrowse);
    phLayout4->addWidget(m_editFilePath);

    QHBoxLayout* phLayout5 = new QHBoxLayout;
    phLayout5->addStretch(1);
    phLayout5->addWidget(m_btnOK);
    phLayout5->addStretch(1);
    phLayout5->addWidget(m_btnCancel);
    phLayout5->addStretch(1);

    QVBoxLayout* pvLayout = new QVBoxLayout;
    pvLayout->setSpacing(4);
    pvLayout->setMargin(0);
    pvLayout->addWidget(m_labHint);
    pvLayout->addLayout(phLayout1);
    pvLayout->addLayout(phLayout2);
    pvLayout->addLayout(phLayout3);
    pvLayout->addLayout(phLayout4);
    pvLayout->addStretch(1);
    pvLayout->addLayout(phLayout5);

    QHBoxLayout* pMainLayout = new QHBoxLayout;
    pMainLayout->setContentsMargins(0, 6, 0, 12);
    pMainLayout->addStretch(1);
    pMainLayout->addLayout(pvLayout);
    pMainLayout->addStretch(1);

    pView->setLayout(pMainLayout);

    RetranslateUI(pView);
}

void CreateDialog::ui_type::RetranslateUI(CreateDialog * pView)
{
    _labUserName->setText("User Name:");
    _labPassword->setText("Password:");
    _labValidate->setText("Validate:");

    m_btnOK->setText("OK");
    m_btnCancel->setText("Cencel");
    m_btnBrowse->setText("File:");
}

QT_END_NAMESPACE
