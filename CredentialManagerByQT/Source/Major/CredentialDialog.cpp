
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenu>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>

#include "bnb_global.h"
#include "Credential/Credential.h"

#include "Major/CredentialDialog.h"

QT_BEGIN_NAMESPACE

CredentialDialog::CredentialDialog(bnb::Credential& src, QWidget * parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint)
    , m_Credential(src)
{
    _ui.SetupUI(this);

    InitView();

    QObject::connect(_ui.m_cboxPlatform, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), this, &CredentialDialog::OnChangedPlatform);
    QObject::connect(_ui.m_cboxAccount, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), this, &CredentialDialog::OnChangedAccount);
    QObject::connect(_ui.m_tabProperty, &QTableWidget::customContextMenuRequested, this, &CredentialDialog::OnTableContextMenu);
    QObject::connect(_ui.m_tabProperty, &QTableWidget::cellDoubleClicked, this, &CredentialDialog::OnDoubleClickedProperty);
    QObject::connect(_ui.m_actEdit, &QAction::triggered, this, &CredentialDialog::OnClickedEdit);
    QObject::connect(_ui.m_actUp, &QAction::triggered, this, &CredentialDialog::OnClickedUp);
    QObject::connect(_ui.m_actDown, &QAction::triggered, this, &CredentialDialog::OnClickedDown);
    QObject::connect(_ui.m_actAdd, &QAction::triggered, this, &CredentialDialog::OnClickedAdd);
    QObject::connect(_ui.m_actRemove, &QAction::triggered, this, &CredentialDialog::OnClickedRemove);
    QObject::connect(_ui.m_btnClose, &QPushButton::clicked, this, &QDialog::accept);
}

void CredentialDialog::InitView()
{
    for (auto ptr_platform = m_Credential.Tree().Head(); ptr_platform; ptr_platform = ptr_platform->m_Next)
        _ui.m_cboxPlatform->addItem(QString::fromStdString(ptr_platform->m_Pair.m_Key.m_Key));
    
    if (0 < _ui.m_cboxPlatform->count())
    {
        _ui.m_cboxPlatform->setCurrentIndex(0);

        if (m_Credential.Tree().Head())
            m_ptrCurAccount = &m_Credential.Tree().Head()->m_Pair.m_Value;
    }
    else
    {
        if (_ui.m_cboxPlatform->isEnabled())
            _ui.m_cboxPlatform->setEnabled(false);
    }

    UpdateAccount();
}

void CredentialDialog::UpdateAccount()
{
    _ui.m_cboxAccount->clear();

    if (m_ptrCurAccount)
    {
        for (auto ptr_account = m_ptrCurAccount->Head(); ptr_account; ptr_account = ptr_account->m_Next)
            _ui.m_cboxAccount->addItem(QString::fromStdString(ptr_account->m_Pair.m_Key.m_Key));

        if (0 < _ui.m_cboxAccount->count())
        {
            _ui.m_cboxAccount->setCurrentIndex(0);

            if (m_ptrCurAccount->Head())
                m_ptrCurProperty = &m_ptrCurAccount->Head()->m_Pair.m_Value;

            if (!_ui.m_cboxAccount->isEnabled())
                _ui.m_cboxAccount->setEnabled(true);
        }
    }
    else
    {
        if (_ui.m_cboxAccount->isEnabled())
            _ui.m_cboxAccount->setEnabled(false);
    }

    UpdateProperty();
}

void CredentialDialog::UpdateProperty()
{
    _ui.m_tabProperty->clearContents();

    if (m_ptrCurProperty)
    {
        _ui.m_tabProperty->setRowCount(m_ptrCurProperty->Size());

        unsigned int nIndex = 0;
        for (auto ptr_property = m_ptrCurProperty->Head(); ptr_property; ptr_property = ptr_property->m_Next)
        {
            QTableWidgetItem* pItemKey = new QTableWidgetItem(QString::fromStdString(ptr_property->m_Pair.m_Key));
            QTableWidgetItem* pItemValue = new QTableWidgetItem(QString::fromStdString(ptr_property->m_Pair.m_Value));
            _ui.m_tabProperty->setItem(nIndex, 0, pItemKey);
            _ui.m_tabProperty->setItem(nIndex, 1, pItemValue);

            ++nIndex;
        }

        _ui.m_tabProperty->setEnabled(true);
        _ui.m_tabProperty->resizeColumnsToContents();
    }
    else
    {
        _ui.m_tabProperty->setEnabled(false);
    }
}

void CredentialDialog::OnChangedPlatform(const QString & strText)
{
    QString strKey = _ui.m_cboxPlatform->currentText();
    m_ptrCurAccount = nullptr;

    if (!strKey.isEmpty())
    {
        auto ptr_platform = m_Credential.Tree().Find(bnb::platform_type(strKey.toStdString()));

        if (ptr_platform)
            m_ptrCurAccount = &ptr_platform->m_Value;
    }

    UpdateAccount();
}

void CredentialDialog::OnChangedAccount(const QString & strText)
{
    QString strKey = _ui.m_cboxAccount->currentText();
    m_ptrCurProperty = nullptr;

    if (m_ptrCurAccount && !strKey.isEmpty())
    {
        auto ptr_account = m_ptrCurAccount->Find(bnb::account_type(strKey.toStdString()));

        if (ptr_account)
            m_ptrCurProperty = &ptr_account->m_Value;
    }

    UpdateProperty();
}

void CredentialDialog::OnDoubleClickedProperty(int row, int cln)
{
}

void CredentialDialog::OnTableContextMenu(const QPoint& pos)
{
    QMenu menu(this);

    menu.addAction(_ui.m_actEdit);
    menu.addSeparator();
    menu.addAction(_ui.m_actAdd);
    menu.addAction(_ui.m_actRemove);
    menu.addSeparator();
    menu.addAction(_ui.m_actUp);
    menu.addAction(_ui.m_actDown);

    menu.exec(QCursor::pos());
}

void CredentialDialog::OnClickedEdit()
{
}

void CredentialDialog::OnClickedUp()
{
}

void CredentialDialog::OnClickedDown()
{
}

void CredentialDialog::OnClickedAdd()
{
}

void CredentialDialog::OnClickedRemove()
{
}

bool CredentialDialog::OnEditAccount(bnb::platform_type * pp, bnb::account_type * pa)
{
    EditAccountDialog dlg(*pp, *pa, this, this);
    if (QDialog::Accepted == dlg.exec())
    {
        SaveCredential();
        return true;
    }

    return false;
}

void CredentialDialog::ui_type::SetupUI(CredentialDialog * pView)
{
    pView->setObjectName("CredentialDialog");
    pView->setWindowIcon(QIcon(":/CredentialManager/Resources/image/credential-dialog.ico"));
    pView->setWindowTitle("Credential");

    _labPlatform = new QLabel(pView);
    _labPlatform->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    _labPlatform->setFixedSize(60, 28);
    _labAccount = new QLabel(pView);
    _labAccount->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    _labAccount->setFixedSize(60, 28);

    m_actEdit = new QAction(pView);
    m_actUp = new QAction(pView);
    m_actDown = new QAction(pView);
    m_actAdd = new QAction(pView);
    m_actRemove = new QAction(pView);
    
    m_cboxPlatform = new QComboBox(pView);
    m_cboxPlatform->setMinimumSize(160, 28);
    m_cboxPlatform->setEditable(false);
    m_cboxAccount = new QComboBox(pView);
    m_cboxAccount->setMinimumSize(160, 28);
    m_cboxAccount->setEditable(false);

    m_tabProperty = new QTableWidget(0, 2, pView);
    m_tabProperty->setMinimumSize(240, 200);
    m_tabProperty->setShowGrid(true);
    m_tabProperty->setContextMenuPolicy(Qt::CustomContextMenu);    
    m_tabProperty->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tabProperty->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tabProperty->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tabProperty->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_tabProperty->verticalHeader()->setDefaultSectionSize(28);
    m_tabProperty->verticalHeader()->setVisible(false);
    m_tabProperty->horizontalHeader()->setFixedHeight(24);
    m_tabProperty->horizontalHeader()->setHighlightSections(false);
    m_tabProperty->horizontalHeader()->setStretchLastSection(true);
    m_tabProperty->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tabProperty->horizontalHeader()->resizeSection(0, 200);
    m_tabProperty->horizontalHeader()->resizeSection(1, 200);

    m_btnClose = new QPushButton(pView);

    QHBoxLayout* phLayout1 = new QHBoxLayout;
    phLayout1->setMargin(0);
    phLayout1->setSpacing(2);
    phLayout1->addWidget(_labPlatform);
    phLayout1->addWidget(m_cboxPlatform, 1);

    QHBoxLayout* phLayout2 = new QHBoxLayout;
    phLayout2->setMargin(0);
    phLayout2->setSpacing(2);
    phLayout2->addWidget(_labAccount);
    phLayout2->addWidget(m_cboxAccount, 1);

    QHBoxLayout* phLayout3 = new QHBoxLayout;
    phLayout3->setMargin(4);
    phLayout3->setSpacing(0);
    phLayout3->addStretch(1);
    phLayout3->addWidget(m_btnClose);
    phLayout3->addStretch(1);

    QVBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->setMargin(4);
    pMainLayout->setSpacing(8);
    pMainLayout->addLayout(phLayout1);
    pMainLayout->addLayout(phLayout2);
    pMainLayout->addWidget(m_tabProperty, 1);
    pMainLayout->addLayout(phLayout3);

    pView->setLayout(pMainLayout);

    RetranslateUI(pView);
}

void CredentialDialog::ui_type::RetranslateUI(CredentialDialog * pView)
{
    _labPlatform->setText("Platform:");
    _labAccount->setText("Account:");

    m_actEdit->setText("Edit");
    m_actUp->setText("Line up");
    m_actDown->setText("Line Down");
    m_actAdd->setText("Add");
    m_actRemove->setText("Remove");

    m_btnClose->setText("Close");

    m_tabProperty->setHorizontalHeaderLabels({ "Key", "Value" });
}

QT_END_NAMESPACE
