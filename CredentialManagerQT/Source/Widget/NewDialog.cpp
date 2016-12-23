#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>

#include "Credential/Credential.h"

#include "credential_qt_string.h"
#include "credential_qt_utils.h"

#include "Widget/NewDialog.h"

QT_BEGIN_NAMESPACE

NewDialog::NewDialog(QWidget * parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
{
    _ui.SetupUI(this);

    QObject::connect(_ui.m_editUserName, &QLineEdit::textChanged, _ui.m_labHint, &QLabel::clear);
    QObject::connect(_ui.m_editPassword, &QLineEdit::textChanged, _ui.m_labHint, &QLabel::clear);
    QObject::connect(_ui.m_editValidate, &QLineEdit::textChanged, _ui.m_labHint, &QLabel::clear);
    QObject::connect(_ui.m_editFilePath, &QLineEdit::textChanged, _ui.m_labHint, &QLabel::clear);
    QObject::connect(_ui.m_editComment, &QLineEdit::textChanged, _ui.m_labHint, &QLabel::clear);
    QObject::connect(_ui.m_btnBrowse, &QPushButton::clicked, this, &NewDialog::OnClickedBrowse);
    QObject::connect(_ui.m_btnOK, &QPushButton::clicked, this, &NewDialog::OnClickedOK);
    QObject::connect(_ui.m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

QString NewDialog::GetFileName() const { return _ui.m_editFilePath->text(); }
QString NewDialog::GetUserName() const { return _ui.m_editUserName->text(); }
QString NewDialog::GetPassword() const { return _ui.m_editPassword->text(); }
QString NewDialog::GetComment() const { return _ui.m_editComment->text(); }

void NewDialog::OnClickedOK()
{
    if (_ui.m_editUserName->text().isEmpty())
    {
        _ui.m_labHint->setText("User Name mustn\'t be null !");
        return;
    }

    if (!bnb::Credential::ValidateName(From_QString(_ui.m_editUserName->text())))
    {
        _ui.m_labHint->setText("The user name you entered is invalidate !");
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

    accept();
}

void NewDialog::OnClickedBrowse()
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

void NewDialog::ui_type::SetupUI(NewDialog * pView)
{
    pView->setObjectName("NewDialog");
    ui_utils::SetBackgroundColor(pView, Qt::white);

    _labUserName = ui_utils::MakeStaticLabel(pView);
    _labPassword = ui_utils::MakeStaticLabel(pView);
    _labValidate = ui_utils::MakeStaticLabel(pView);
    _labComment = ui_utils::MakeStaticLabel(pView);

    m_labHint = ui_utils::MakeDynamicLabel(pView, Qt::red, Qt::AlignCenter);

    m_editUserName = ui_utils::MakeLineEdit(pView, ui_utils::edit_default_w);
    m_editPassword = ui_utils::MakePasswordLine(pView, ui_utils::edit_default_w);
    m_editValidate = ui_utils::MakePasswordLine(pView, ui_utils::edit_default_w);
    m_editComment = ui_utils::MakeLineEdit(pView, ui_utils::edit_default_w);
    m_editFilePath = ui_utils::MakeLineEdit(pView, ui_utils::edit_default_w);

    m_btnBrowse = new QPushButton(pView);
    m_btnBrowse->setFixedHeight(ui_utils::def_widget_h);
    m_btnBrowse->setFont(ui_utils::MakeFont());
    m_btnBrowse->setStyleSheet(
        "QPushButton{ background-color:transparent; border:none; color:#FF4000; text-align:right; }\n"
        "QPushButton:hover{ color:#40B040; }\n"
        "QPushButton:pressed{ color:#4040FF; }");

    m_btnOK = ui_utils::MakeButton(pView);
    m_btnCancel = ui_utils::MakeButton(pView);

    m_editUserName->setFocus();
    m_editUserName->setMaxLength(ui_utils::def_text_length);
    m_editPassword->setMaxLength(ui_utils::def_text_length);
    m_editValidate->setMaxLength(ui_utils::def_text_length);
    m_editFilePath->setReadOnly(true);
    m_btnOK->setDefault(true);

    QVBoxLayout* pvLayout1 = new QVBoxLayout;
    QVBoxLayout* pvLayout2 = new QVBoxLayout;
    QVBoxLayout* pvLayout3 = new QVBoxLayout;
    pvLayout1->setMargin(0);
    pvLayout2->setMargin(0);
    pvLayout3->setMargin(0);
    pvLayout1->setSpacing(2);
    pvLayout2->setSpacing(2);
    pvLayout3->setSpacing(2);
    pvLayout1->addWidget(_labUserName);
    pvLayout1->addWidget(_labPassword);
    pvLayout1->addWidget(_labValidate);
    pvLayout1->addWidget(_labComment);
    pvLayout1->addWidget(m_btnBrowse);
    pvLayout2->addWidget(m_editUserName);
    pvLayout2->addWidget(m_editPassword);
    pvLayout2->addWidget(m_editValidate);
    pvLayout2->addWidget(m_editComment);
    pvLayout2->addWidget(m_editFilePath);
    pvLayout3->addWidget(ui_utils::MakeMarkLabel(pView));
    pvLayout3->addWidget(ui_utils::MakeMarkLabel(pView));
    pvLayout3->addWidget(ui_utils::MakeMarkLabel(pView));
    pvLayout3->addStretch(1);
    pvLayout3->addWidget(ui_utils::MakeMarkLabel(pView));

    QHBoxLayout* phLayout1 = new QHBoxLayout;
    phLayout1->setSpacing(0);
    phLayout1->setMargin(0);
    phLayout1->addLayout(pvLayout1);
    phLayout1->addLayout(pvLayout2);
    phLayout1->addLayout(pvLayout3);

    QHBoxLayout* phLayout2 = ui_utils::MakeButtonLayout(m_btnOK, m_btnCancel);

    QVBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->setMargin(2);
    pMainLayout->setSpacing(4);
    pMainLayout->addWidget(m_labHint);
    pMainLayout->addLayout(phLayout1);
    pMainLayout->addLayout(phLayout2);

    RetranslateUI(pView);

    pView->setLayout(pMainLayout);
    pView->setFixedSize(pView->sizeHint());
}

void NewDialog::ui_type::RetranslateUI(NewDialog * pView)
{
    _labUserName->setText("User Name: ");
    _labPassword->setText("Password: ");
    _labValidate->setText("Validate: ");
    _labComment->setText("Description: ");

    m_btnOK->setText("OK");
    m_btnCancel->setText("Cencel");
    m_btnBrowse->setText("File: ");
    
    m_editUserName->setPlaceholderText("input your name");
    m_editPassword->setPlaceholderText("input a password");
    m_editValidate->setPlaceholderText("input the password again");
    m_editFilePath->setPlaceholderText("select a new file");
    m_editComment->setPlaceholderText("input a description");
}

QT_END_NAMESPACE
