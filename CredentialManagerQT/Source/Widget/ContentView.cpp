#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtCore/QDateTime>
#include <QtGui/QGuiApplication>

#include "credential_qt_utils.h"

#include "Credential/Credential.h"

#include "Widget/ContentView.h"

static inline QTableWidgetItem* MakeTableItem(const QString& strText, unsigned int id, const QColor& c = Qt::black, Qt::Alignment a = Qt::AlignCenter)
{
    QTableWidgetItem* pItem = new QTableWidgetItem(strText);
    pItem->setTextAlignment(a);
    pItem->setTextColor(c);
    pItem->setData(Qt::UserRole, id);

    QFont font = QGuiApplication::font();
    font.setPointSize(10);
    pItem->setFont(font);

    return pItem;
}

//==============================================================================
// Implementation of CredentialView
//==============================================================================
CredentialView::CredentialView(bnb::Credential& credential, QWidget * parent)
    : base_type(credential.GetID(), parent)
    , m_Credential(credential)
{
    UpdateInfo();
    UpdateTable();
}

void CredentialView::UpdateInfo()
{
    _ui.m_labText[0]->setText(QDateTime::fromTime_t(m_Credential.GetTime()).toString("yyyy-MM-dd HH:mm:ss"));
    _ui.m_labText[1]->setText(QString::fromStdString(m_Credential.GetUser()));
    _ui.m_labText[2]->setText("-");
}

void CredentialView::UpdateTable()
{
    _ui.m_tabView->clearContents();

    unsigned int nRows = m_Credential.Tree().Size();
    _ui.m_tabView->setRowCount(nRows);
    _ui.m_tabView->setEnabled(0 < nRows);

    if (0 < nRows)
    {
        unsigned int nIndex = 0;
        for (auto ptr = m_Credential.Tree().Head(); ptr; ptr = ptr->m_Next)
        {
            auto pName = MakeTableItem(QString::fromStdString(ptr->m_Pair.m_Key.m_strName), ptr->m_Pair.m_Key.m_ID, { 224, 32, 32 });
            auto pUrl = MakeTableItem(QString::fromStdString(ptr->m_Pair.m_Key.m_strUrl), ptr->m_Pair.m_Key.m_ID, { 64, 64, 255 });
            auto pDisplay = MakeTableItem(QString::fromStdString(ptr->m_Pair.m_Key.m_strDisplay), ptr->m_Pair.m_Key.m_ID, { 32, 160, 32 });

            _ui.m_tabView->setItem(nIndex, 0, pName);
            _ui.m_tabView->setItem(nIndex, 1, pUrl);
            _ui.m_tabView->setItem(nIndex, 2, pDisplay);

            ++nIndex;
        }

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
                for (auto ptr = m_Credential.Tree().Head(); ptr; ptr = ptr->m_Next)
                {
                    if (ptr->m_Pair.m_Key.m_ID == id)
                    {
                        pItem->setText(QString::fromStdString(ptr->m_Pair.m_Key.m_strName));
                        _ui.m_tabView->item(i, 1)->setText(QString::fromStdString(ptr->m_Pair.m_Key.m_strUrl));
                        _ui.m_tabView->item(i, 2)->setText(QString::fromStdString(ptr->m_Pair.m_Key.m_strDisplay));
                        break;
                    }
                }

                break;
            }
        }
    }
}

//------------------------------------------------------------------------------

template<>
void CredentialView::base_type::ui_type::RetranslateUI(QWidget* pView)
{
    m_tabView->setColumnCount(3);
    m_tabView->setHorizontalHeaderLabels({ "    Platform    ", "    Url    ", "    Display    " });

    _labText[0]->setText("Update Time: ");
    _labText[1]->setText("User Name: ");
    _labText[2]->setText("Display: ");
}

template<>
void CredentialView::base_type::ui_type::CreateLabel()
{
    for (unsigned int i = 0; i < 3; ++i)
    {
        _labText[i] = ui_utils::MakeStaticLabel(_viewCentral, 96, 24, ui_utils::g_clrLabel, false);
        m_labText[i] = ui_utils::MakeDynamicLabel(_viewCentral, ui_utils::g_clrCredential, false);
    }
}

//==============================================================================
// Implementation of PlatformView
//==============================================================================
PlatformView::PlatformView(const bnb::platform_tree::data_type& tp, QWidget * parent)
    : base_type(tp.m_Key.m_ID, parent)
    , m_Platform(tp)
{
    UpdateInfo();
    UpdateTable();
}

void PlatformView::UpdateInfo()
{
    _ui.m_labText[0]->setText(QString::fromStdString(m_Platform.m_Key.m_strName));
    _ui.m_labText[1]->setText(QString::fromStdString(m_Platform.m_Key.m_strUrl));
    _ui.m_labText[2]->setText(QString::fromStdString(m_Platform.m_Key.m_strDisplay));
}

void PlatformView::UpdateTable()
{
    _ui.m_tabView->clearContents();

    unsigned int nRows = m_Platform.m_Value.Size();
    _ui.m_tabView->setRowCount(nRows);
    _ui.m_tabView->setEnabled(0 < nRows);

    if (0 < nRows)
    {
        unsigned int nIndex = 0;
        for (auto ptr = m_Platform.m_Value.Head(); ptr; ptr = ptr->m_Next)
        {
            auto pName = MakeTableItem(QString::fromStdString(ptr->m_Pair.m_Key.m_strName), ptr->m_Pair.m_Key.m_ID, { 64, 64, 255 });
            auto pDisplay = MakeTableItem(QString::fromStdString(ptr->m_Pair.m_Key.m_strDisplay), ptr->m_Pair.m_Key.m_ID, { 32, 160, 32 });

            _ui.m_tabView->setItem(nIndex, 0, pName);
            _ui.m_tabView->setItem(nIndex, 1, pDisplay);

            ++nIndex;
        }

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
                for (auto ptr = m_Platform.m_Value.Head(); ptr; ptr = ptr->m_Next)
                {
                    if (ptr->m_Pair.m_Key.m_ID == id)
                    {
                        pItem->setText(QString::fromStdString(ptr->m_Pair.m_Key.m_strName));
                        _ui.m_tabView->item(i, 1)->setText(QString::fromStdString(ptr->m_Pair.m_Key.m_strDisplay));
                        break;
                    }
                }

                break;
            }
        }
    }
}

//------------------------------------------------------------------------------

template<>
void PlatformView::base_type::ui_type::RetranslateUI(QWidget* pView)
{
    m_tabView->setColumnCount(2);
    m_tabView->setHorizontalHeaderLabels({ "    Account    ", "    Display    " });

    _labText[0]->setText("Platform: ");
    _labText[1]->setText("Url: ");
    _labText[2]->setText("Display: ");
}

template<>
void PlatformView::base_type::ui_type::CreateLabel()
{
    for (unsigned int i = 0; i < 3; ++i)
    {
        _labText[i] = ui_utils::MakeStaticLabel(_viewCentral, ui_utils::lab_default_w, ui_utils::lab_default_h, ui_utils::g_clrLabel, false);
        m_labText[i] = ui_utils::MakeDynamicLabel(_viewCentral, ui_utils::g_clrPlatform, false);
    }
}

//==============================================================================
// Implementation of AccountView
//==============================================================================
AccountView::AccountView(const bnb::account_tree::data_type & tp, QWidget * parent)
    : base_type(tp.m_Key.m_ID, parent)
    , m_Account(tp)
{
    UpdateInfo();
    UpdateTable();
}

void AccountView::UpdateInfo()
{
    _ui.m_labText[0]->setText(QString::fromStdString(m_Account.m_Key.m_strName));
    _ui.m_labText[1]->setText(QString::fromStdString(m_Account.m_Key.m_strDisplay));
}

void AccountView::UpdateTable()
{
    _ui.m_tabView->clearContents();

    unsigned int nRows = m_Account.m_Value.Size();
    _ui.m_tabView->setRowCount(nRows);
    _ui.m_tabView->setEnabled(0 < nRows);

    if (0 < nRows)
    {
        unsigned int nIndex = 0;
        m_Account.m_Value.Foreach([this, &nIndex](const bnb::property_tree::data_type& property) {
            auto pKey = MakeTableItem(QString::fromStdString(property.m_Key.m_strName), property.m_Key.m_ID, { 32, 160, 32 });
            auto pValue = MakeTableItem(QString::fromStdString(property.m_Value.m_strName), property.m_Key.m_ID, { 32, 160, 32 });

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
                m_Account.m_Value.Action([this, pItem, id, i](const bnb::property_tree::data_type& property) {
                    if (property.m_Key.m_ID == id)
                    {
                        pItem->setText(QString::fromStdString(property.m_Key.m_strName));
                        _ui.m_tabView->item(i, 1)->setText(QString::fromStdString(property.m_Value.m_strName));
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
void AccountView::base_type::ui_type::RetranslateUI(QWidget* pView)
{
    m_tabView->setColumnCount(2);
    m_tabView->setHorizontalHeaderLabels({ "    Key    ", "    Value    " });

    _labText[0]->setText("Account: ");
    _labText[1]->setText("Display: ");
}

template<>
void AccountView::base_type::ui_type::CreateLabel()
{
    for (unsigned int i = 0; i < 2; ++i)
    {
        _labText[i] = ui_utils::MakeStaticLabel(_viewCentral, ui_utils::lab_default_w, ui_utils::lab_default_h, ui_utils::g_clrLabel, false);
        m_labText[i] = ui_utils::MakeDynamicLabel(_viewCentral, ui_utils::g_clrAccount, false);
    }
}
