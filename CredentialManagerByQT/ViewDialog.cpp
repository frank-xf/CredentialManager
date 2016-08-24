
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QPushButton>

#include "../CredentialManager/bnb_global.h"
#include "../CredentialManager/Credential/Credential.h"

#include "ViewDialog.h"

QT_BEGIN_NAMESPACE

ViewDialog::ViewDialog(bnb::Credential& src, QWidget * parent)
    : QDialog(parent)
    , m_Credential(src)
{
    _ui.SetupUI(this);

    QObject::connect(_ui.m_cboxPlatform, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), this, &ViewDialog::OnChangedPlatform);
    QObject::connect(_ui.m_cboxAccount, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), this, &ViewDialog::OnChangedAccount);
    QObject::connect(_ui.m_tabProperty, &QTableWidget::cellDoubleClicked, this, &ViewDialog::OnDoubleClickedProperty);
    QObject::connect(_ui.m_tabProperty, &QTableWidget::currentCellChanged, this, &ViewDialog::OnChangedCurrentProperty);
    QObject::connect(_ui.m_btnUp, &QPushButton::clicked, this, &ViewDialog::OnClickedUp);
    QObject::connect(_ui.m_btnDown, &QPushButton::clicked, this, &ViewDialog::OnClickedDown);
    QObject::connect(_ui.m_btnAdd, &QPushButton::clicked, this, &ViewDialog::OnClickedAdd);
    QObject::connect(_ui.m_btnRemove, &QPushButton::clicked, this, &ViewDialog::OnClickedRemove);
    QObject::connect(_ui.m_btnClose, &QPushButton::clicked, this, &QDialog::accept);
}

void ViewDialog::InitView()
{
}

void ViewDialog::OnChangedPlatform(const QString & strText)
{
}

void ViewDialog::OnChangedAccount(const QString & strText)
{
}

void ViewDialog::OnDoubleClickedProperty(int row, int cln)
{
}

void ViewDialog::OnChangedCurrentProperty(int curRow, int curCln, int preRow, int preCln)
{
}

void ViewDialog::OnClickedUp()
{
}

void ViewDialog::OnClickedDown()
{
}

void ViewDialog::OnClickedAdd()
{
}

void ViewDialog::OnClickedRemove()
{
}

void ViewDialog::ui_type::SetupUI(ViewDialog * pView)
{

    RetranslateUI(pView);
}

void ViewDialog::ui_type::RetranslateUI(ViewDialog * pView)
{
}

QT_END_NAMESPACE
