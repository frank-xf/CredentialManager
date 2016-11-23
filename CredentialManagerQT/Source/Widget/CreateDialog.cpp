#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>

#include "Credential/Credential.h"

#include "credential_qt_string.h"
#include "credential_qt_utils.h"
#include "credential_qt_manager.h"
#include "credential_model_manager.h"

#include "Widget/CreateDialog.h"

QT_BEGIN_NAMESPACE

CreateDialog::CreateDialog(QWidget * parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
{
    _ui.SetupUI(this);

    QObject::connect(_ui.m_editUserName, &QLineEdit::textChanged, _ui.m_labHint, &QLabel::clear);
    QObject::connect(_ui.m_editPassword, &QLineEdit::textChanged, _ui.m_labHint, &QLabel::clear);
    QObject::connect(_ui.m_editValidate, &QLineEdit::textChanged, _ui.m_labHint, &QLabel::clear);
    QObject::connect(_ui.m_editFilePath, &QLineEdit::textChanged, _ui.m_labHint, &QLabel::clear);
    QObject::connect(_ui.m_editComment, &QLineEdit::textChanged, _ui.m_labHint, &QLabel::clear);
    QObject::connect(_ui.m_btnBrowse, &QPushButton::clicked, this, &CreateDialog::OnClickedBrowse);
    QObject::connect(_ui.m_btnOK, &QPushButton::clicked, this, &CreateDialog::OnClickedOK);
    QObject::connect(_ui.m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

void CreateDialog::OnClickedOK()
{
    if (_ui.m_editUserName->text().isEmpty())
    {
        _ui.m_labHint->setText("User Name mustn\'t be null !");
        return;
    }

    if (!ui_utils::ValidateName(_ui.m_editUserName->text()))
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

	g_AppMgr.Model().Data().Clear();
    g_AppMgr.Model().Data().SetWord(From_QString(s1));
	g_AppMgr.Model().Data().SetUser(From_QString(_ui.m_editUserName->text()));
    g_AppMgr.Model().Data().SetComment(From_QString(_ui.m_editComment->text()));
	g_AppMgr.Model().SetFile(_ui.m_editFilePath->text());
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
    pView->setObjectName("CreateDialog");
    ui_utils::SetBackgroundColor(pView, Qt::white);

    _labUserName = ui_utils::MakeStaticLabel(pView);
    _labPassword = ui_utils::MakeStaticLabel(pView);
    _labValidate = ui_utils::MakeStaticLabel(pView);
    _labComment = ui_utils::MakeStaticLabel(pView);

    m_labHint = ui_utils::MakeDynamicLabel(pView, Qt::red, Qt::AlignCenter);

    m_editUserName = ui_utils::MakeLineEdit(pView, ui_utils::edit_path_w);
    m_editPassword = ui_utils::MakePasswordLine(pView, ui_utils::edit_path_w);
    m_editValidate = ui_utils::MakePasswordLine(pView, ui_utils::edit_path_w);
    m_editComment = ui_utils::MakeLineEdit(pView, ui_utils::edit_path_w);
    m_editFilePath = ui_utils::MakeLineEdit(pView, ui_utils::edit_path_w);

    m_btnBrowse = new QPushButton(pView);
    m_btnBrowse->setFixedSize(ui_utils::lab_text_w, ui_utils::def_widget_h);
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

    QHBoxLayout* phLayout1 = new QHBoxLayout;
    phLayout1->setSpacing(0);
    phLayout1->setMargin(0);
    phLayout1->addWidget(_labUserName);
    phLayout1->addWidget(m_editUserName);
    phLayout1->addWidget(ui_utils::MakeMarkLabel(pView));

    QHBoxLayout* phLayout2 = new QHBoxLayout;
    phLayout2->setSpacing(0);
    phLayout2->setMargin(0);
    phLayout2->addWidget(_labPassword);
    phLayout2->addWidget(m_editPassword);
    phLayout2->addWidget(ui_utils::MakeMarkLabel(pView));

    QHBoxLayout* phLayout3 = new QHBoxLayout;
    phLayout3->setSpacing(0);
    phLayout3->setMargin(0);
    phLayout3->addWidget(_labValidate);
    phLayout3->addWidget(m_editValidate);
    phLayout3->addWidget(ui_utils::MakeMarkLabel(pView));

    QHBoxLayout* phLayout4 = new QHBoxLayout;
    phLayout4->setSpacing(0);
    phLayout4->setMargin(0);
    phLayout4->addWidget(_labComment);
    phLayout4->addWidget(m_editComment);
    phLayout4->addStretch(1);

    QHBoxLayout* phLayout5 = new QHBoxLayout;
    phLayout5->setSpacing(0);
    phLayout5->setMargin(0);
    phLayout5->addWidget(m_btnBrowse);
    phLayout5->addWidget(m_editFilePath);
    phLayout5->addWidget(ui_utils::MakeMarkLabel(pView));

    QHBoxLayout* phLayout6 = new QHBoxLayout;
    phLayout6->setContentsMargins(0, 12, 0, 12);
    phLayout6->setSpacing(0);
    phLayout6->addStretch(1);
    phLayout6->addWidget(m_btnOK);
    phLayout6->addStretch(1);
    phLayout6->addWidget(m_btnCancel);
    phLayout6->addStretch(1);

    QVBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->setMargin(2);
    pMainLayout->setSpacing(4);
    pMainLayout->addWidget(m_labHint);
    pMainLayout->addLayout(phLayout1);
    pMainLayout->addLayout(phLayout2);
    pMainLayout->addLayout(phLayout3);
    pMainLayout->addLayout(phLayout4);
    pMainLayout->addLayout(phLayout5);
    pMainLayout->addLayout(phLayout6);
    
    pView->setLayout(pMainLayout);
    pView->setFixedSize(pView->sizeHint());

    RetranslateUI(pView);
}

void CreateDialog::ui_type::RetranslateUI(CreateDialog * pView)
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
