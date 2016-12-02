#include <QtCore/QDateTime>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTableWidget>

#include "Credential/Credential.h"

#include "credential_qt_string.h"
#include "credential_qt_utils.h"

#include "Widget/DropTable.h"
#include "Widget/ContentView.h"

QT_BEGIN_NAMESPACE

static inline QTableWidgetItem* MakeTableItem(const QString& strText, unsigned int id, const QColor& c = Qt::black, Qt::Alignment a = Qt::AlignVCenter | Qt::AlignLeft)
{
    QTableWidgetItem* pItem = new QTableWidgetItem(strText);
    pItem->setTextAlignment(a);
    pItem->setTextColor(c);
    pItem->setData(Qt::UserRole, id);
    pItem->setFont(ui_utils::MakeFont());

    return pItem;
}

//==============================================================================
// Implementation of CredentialView
//==============================================================================
CredentialView::CredentialView(const bnb::Credential& credential, QWidget * parent)
    : base_type(credential.GetID(), parent)
    , m_Credential(credential)
{
    UpdateInfo();
    UpdateTable();
}

void CredentialView::UpdateInfo()
{
    _ui.m_editText[0]->setText(QDateTime::fromTime_t(m_Credential.GetTime()).toString("yyyy-MM-dd HH:mm:ss"));
    _ui.m_editText[1]->setText(To_QString(m_Credential.GetUser()));
    _ui.m_editText[2]->setText(To_QString(m_Credential.GetComment()));
}

void CredentialView::UpdateTable()
{
    _ui.m_tabView->clearContents();

    unsigned int nRows = m_Credential.List().Size();
    _ui.m_tabView->setRowCount(nRows);

    if (0 < nRows)
    {
        unsigned int nIndex = 0;
        m_Credential.List().PreorderTraversal([this, &nIndex](const bnb::platform_list::data_type& platform) mutable {
            auto pName = MakeTableItem(To_QString(platform.m_Key.m_strName), platform.m_Key.GetID(), { 255, 64, 0 }, Qt::AlignCenter);
            auto pUrl = MakeTableItem(To_QString(platform.m_Key.m_strUrl), platform.m_Key.GetID(), { 64, 64, 255 });
            auto pDisplay = MakeTableItem(To_QString(platform.m_Key.m_strComment), platform.m_Key.GetID(), { 32, 160, 32 });

            _ui.m_tabView->setItem(nIndex, 0, pName);
            _ui.m_tabView->setItem(nIndex, 1, pUrl);
            _ui.m_tabView->setItem(nIndex, 2, pDisplay);

            ++nIndex;
        });

        _ui.m_tabView->resizeColumnsToContents();
    }
}

void CredentialView::UpdateTable(unsigned int id)
{
    for (int i = 0; i < _ui.m_tabView->rowCount(); ++i)
    {
        if (auto pItem = _ui.m_tabView->item(i, 0))
        {
            if (id == pItem->data(Qt::UserRole).toUInt())
            {
                m_Credential.List().PreorderAction([this, pItem, id, i](const bnb::platform_list::data_type& platform) {
                    if (platform.m_Key.GetID() == id)
                    {
                        pItem->setText(To_QString(platform.m_Key.m_strName));
                        _ui.m_tabView->item(i, 1)->setText(To_QString(platform.m_Key.m_strUrl));
                        _ui.m_tabView->item(i, 2)->setText(To_QString(platform.m_Key.m_strComment));
                        return true;
                    }

                    return false;
                });

                break;
            }
        }
    }
}

//------------------------------------------------------------------------------

template<>
void CredentialView::base_type::ui_type::RetranslateUI(CredentialView::base_type* pView)
{
    m_tabView->setColumnCount(3);
    m_tabView->setHorizontalHeaderLabels({ "    Platform    ", "        Url        ", "        Description        " });

    _labText[0]->setText("Update Time: ");
    _labText[1]->setText("User Name: ");
    _labText[2]->setText("Description: ");
}

template<>
void CredentialView::base_type::ui_type::CreateLabel(CredentialView::base_type* pView)
{
    for (unsigned int i = 0; i < 3; ++i)
    {
        _labText[i] = ui_utils::MakeStaticLabel(pView);
        m_editText[i] = ui_utils::MakeShowLine(pView, ui_utils::g_clrCredential);
    }
}

//==============================================================================
// Implementation of PlatformView
//==============================================================================
PlatformView::PlatformView(const bnb::platform_list::data_type& tp, QWidget * parent)
    : base_type(tp.m_Key.GetID(), parent)
    , m_Platform(tp)
{
    UpdateInfo();
    UpdateTable();
}

void PlatformView::UpdateInfo()
{
    _ui.m_editText[0]->setText(To_QString(m_Platform.m_Key.m_strName));
    _ui.m_editText[1]->setText(To_QString(m_Platform.m_Key.m_strUrl));
    _ui.m_editText[2]->setText(To_QString(m_Platform.m_Key.m_strComment));
}

void PlatformView::UpdateTable()
{
    _ui.m_tabView->clearContents();

    unsigned int nRows = m_Platform.m_Value.Size();
    _ui.m_tabView->setRowCount(nRows);

    if (0 < nRows)
    {
        unsigned int nIndex = 0;
        m_Platform.m_Value.PreorderTraversal([this, &nIndex](const bnb::account_list::data_type& account) mutable {
            auto pName = MakeTableItem(To_QString(account.m_Key.m_strName), account.m_Key.GetID(), { 64, 64, 255 }, Qt::AlignCenter);
            auto pDisplay = MakeTableItem(To_QString(account.m_Key.m_strComment), account.m_Key.GetID(), { 32, 160, 32 });

            _ui.m_tabView->setItem(nIndex, 0, pName);
            _ui.m_tabView->setItem(nIndex, 1, pDisplay);

            ++nIndex;
        });

        _ui.m_tabView->resizeColumnsToContents();
    }
}

void PlatformView::UpdateTable(unsigned int id)
{
    for (int i = 0; i < _ui.m_tabView->rowCount(); ++i)
    {
        if (auto pItem = _ui.m_tabView->item(i, 0))
        {
            if (id == pItem->data(Qt::UserRole).toUInt())
            {
                m_Platform.m_Value.PreorderAction([this, pItem, id, i](const bnb::account_list::data_type& account) {
                    if (account.m_Key.GetID() == id)
                    {
                        pItem->setText(To_QString(account.m_Key.m_strName));
                        _ui.m_tabView->item(i, 1)->setText(To_QString(account.m_Key.m_strComment));
                        return true;
                    }

                    return false;
                });

                break;
            }
        }
    }
}

//------------------------------------------------------------------------------

template<>
void PlatformView::base_type::ui_type::RetranslateUI(PlatformView::base_type* pView)
{
    m_tabView->setColumnCount(2);
    m_tabView->setHorizontalHeaderLabels({ "        Account        ", "        Description        " });

    _labText[0]->setText("Platform: ");
    _labText[1]->setText("Url: ");
    _labText[2]->setText("Description: ");
}

template<>
void PlatformView::base_type::ui_type::CreateLabel(PlatformView::base_type* pView)
{
    for (unsigned int i = 0; i < 3; ++i)
    {
        _labText[i] = ui_utils::MakeStaticLabel(pView);
        m_editText[i] = ui_utils::MakeShowLine(pView, ui_utils::g_clrPlatform);
    }
}

//==============================================================================
// Implementation of AccountView
//==============================================================================
AccountView::AccountView(const bnb::account_list::data_type & tp, QWidget * parent)
    : base_type(tp.m_Key.GetID(), parent)
    , m_Account(tp)
{
    UpdateInfo();
    UpdateTable();
}

void AccountView::UpdateInfo()
{
    _ui.m_editText[0]->setText(To_QString(m_Account.m_Key.m_strName));
    _ui.m_editText[1]->setText(To_QString(m_Account.m_Key.m_strComment));
}

void AccountView::UpdateTable()
{
    _ui.m_tabView->clearContents();

    unsigned int nRows = m_Account.m_Value.Size();
    _ui.m_tabView->setRowCount(nRows);

    if (0 < nRows)
    {
        unsigned int nIndex = 0;
        m_Account.m_Value.PreorderTraversal([this, &nIndex](const bnb::property_list::data_type& property) mutable {
            auto pKey = MakeTableItem(To_QString(property.m_Key.m_strName), property.m_Key.GetID(), { 32, 160, 32 }, Qt::AlignCenter);
            auto pValue = MakeTableItem(To_QString(property.m_Value.m_strName), property.m_Key.GetID(), { 32, 160, 32 }, Qt::AlignCenter);

            _ui.m_tabView->setItem(nIndex, 0, pKey);
            _ui.m_tabView->setItem(nIndex, 1, pValue);

            ++nIndex;
        });

        _ui.m_tabView->resizeColumnsToContents();
    }
}

void AccountView::UpdateTable(unsigned int id)
{
    for (int i = 0; i < _ui.m_tabView->rowCount(); ++i)
    {
        if (auto pItem = _ui.m_tabView->item(i, 0))
        {
            if (id == pItem->data(Qt::UserRole).toUInt())
            {
                m_Account.m_Value.PreorderAction([this, pItem, id, i](const bnb::property_list::data_type& property) {
                    if (property.m_Key.GetID() == id)
                    {
                        pItem->setText(To_QString(property.m_Key.m_strName));
                        _ui.m_tabView->item(i, 1)->setText(To_QString(property.m_Value.m_strName));
                        return true;
                    }

                    return false;
                });

                break;
            }
        }
    }
}

//------------------------------------------------------------------------------

template<>
void AccountView::base_type::ui_type::RetranslateUI(AccountView::base_type* pView)
{
    m_tabView->setColumnCount(2);
    m_tabView->setHorizontalHeaderLabels({ "        Key        ", "        Value        " });

    _labText[0]->setText("Account: ");
    _labText[1]->setText("Description: ");
}

template<>
void AccountView::base_type::ui_type::CreateLabel(AccountView::base_type* pView)
{
    for (unsigned int i = 0; i < 2; ++i)
    {
        _labText[i] = ui_utils::MakeStaticLabel(pView);
        m_editText[i] = ui_utils::MakeShowLine(pView, ui_utils::g_clrAccount);
    }
}

QT_END_NAMESPACE
