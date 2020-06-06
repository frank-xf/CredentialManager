#include <QtCore/QDateTime>
#include <QtGui/QDropEvent>
#include <QtWidgets/QMenu>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QHeaderView>

#include "../../cm-core/cm_core.h"

#include "../cm_view_delegate.h"
#include "../cm_view_utils.h"

#include "../widgets/DropTable.h"
#include "NodeView.h"

QT_BEGIN_NAMESPACE

inline QTableWidgetItem* MakeTableItem(const QString& strText, const QColor& color, const void* data)
{
    QTableWidgetItem* pItem = new QTableWidgetItem(strText);
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setTextColor(color);
    pItem->setData(Qt::UserRole, PointerToQVariant(data));
    pItem->setFont(MakeFont());

    return pItem;
}

NodeView::NodeView(QWidget* parent)
    : QWidget(parent)
{
    _ui.SetupUI(this);

    QObject::connect(_ui.m_btnEdit, &QPushButton::clicked, this, &NodeView::OnClickedEdit);
    QObject::connect(_ui.m_btnRemove, &QPushButton::clicked, this, &NodeView::OnClickedRemove);

    QObject::connect(_ui.m_tableView->_ui.m_actAdd, &QAction::triggered, this, &NodeView::OnAdd);
    QObject::connect(_ui.m_tableView->_ui.m_actEdit, &QAction::triggered, this, &NodeView::OnEdit);
    QObject::connect(_ui.m_tableView->_ui.m_actRemove, &QAction::triggered, this, &NodeView::OnRemove);
    QObject::connect(_ui.m_tableView->_ui.m_actMoveUp, &QAction::triggered, this, &NodeView::OnMoveUp);
    QObject::connect(_ui.m_tableView->_ui.m_actMoveDown, &QAction::triggered, this, &NodeView::OnMoveDown);
    QObject::connect(_ui.m_tableView->horizontalHeader(), &QHeaderView::sortIndicatorChanged, this, &NodeView::OnSort);

    QObject::connect(_ui.m_tableView, &DropTable::moveItem, this, &NodeView::OnMoveItem);
    QObject::connect(_ui.m_tableView, &DropTable::doubleClicked, this, &NodeView::OnDoubleClicked);
}

void NodeView::Show(const xf::credential::credential_t& credential)
{
    if (xf::credential::credential_t::type != _ct)
    {
        _ct = xf::credential::credential_t::type;

        _ui.AdjustIndicator(0);
        _ui.AdjustLabel(3);

        const char* labels[3]{ "Last Update: ", "Username: ", "Description: " };
        for (unsigned int i = 0; i < 3; ++i)
            _ui.m_Displays[i].first->setText(labels[i]);
    }

    _ui.m_Displays[0].second->setText(QDateTime::fromTime_t(credential.Time()).toString("yyyy-MM-dd HH:mm:ss"));
    _ui.m_Displays[1].second->setText(ToQString(credential.Username()));
    _ui.m_Displays[2].second->setText(ToQString(credential.Description()));

    _ui.m_tableView->clearContents();

    if (unsigned int nRows = credential.Size(); 0 < nRows)
    {
        _ui.m_tableView->setRowCount(nRows);
        _ui.m_tableView->setColumnCount(3);
        _ui.m_tableView->setHorizontalHeaderLabels({ "    Platform    ", "        Url        ", "        Description        " });

        unsigned int nIndex = 0;
        credential.Traversal([this, &nIndex](const xf::credential::platform_t& platform) mutable {
            auto pName = MakeTableItem(ToQString(platform.Item().name), { 255, 64, 0 }, &platform);
            auto pUrl = MakeTableItem(ToQString(platform.Item().url), { 64, 64, 255 }, &platform);
            auto pDisplay = MakeTableItem(ToQString(platform.Item().description), { 32, 160, 32 }, &platform);

            _ui.m_tableView->setItem(nIndex, 0, pName);
            _ui.m_tableView->setItem(nIndex, 1, pUrl);
            _ui.m_tableView->setItem(nIndex, 2, pDisplay);

            ++nIndex;
        });
    }
}

void NodeView::Show(const xf::credential::platform_t& platform)
{
    if (xf::credential::platform_t::item_type::type != _ct)
    {
        _ct = xf::credential::platform_t::item_type::type;

        _ui.AdjustIndicator(2);
        _ui.AdjustLabel(4);

        const char* labels[4]{ "Last Update: ", "Platform: ", "Url: ", "Description: " };
        for (unsigned int i = 0; i < 4; ++i)
            _ui.m_Displays[i].first->setText(labels[i]);
    }

    _ui.m_Displays[0].second->setText(QDateTime::fromTime_t(platform.Item().Time()).toString("yyyy-MM-dd HH:mm:ss"));
    _ui.m_Displays[1].second->setText(ToQString(platform.Item().name));
    _ui.m_Displays[2].second->setText(ToQString(platform.Item().url));
    _ui.m_Displays[3].second->setText(ToQString(platform.Item().description));

    _ui.m_tableView->clearContents();

    if (unsigned int nRows = platform.Size(); 0 < nRows)
    {
        _ui.m_tableView->setRowCount(nRows);
        _ui.m_tableView->setColumnCount(2);
        _ui.m_tableView->setHorizontalHeaderLabels({ "    Account    ", "        Description        " });

        unsigned int nIndex = 0;
        platform.Traversal([this, &nIndex](const xf::credential::account_t& account) mutable {
            auto pName = MakeTableItem(ToQString(account.Item().name), { 255, 64, 0 }, &account);
            auto pDisplay = MakeTableItem(ToQString(account.Item().description), { 32, 160, 32 }, &account);

            _ui.m_tableView->setItem(nIndex, 0, pName);
            _ui.m_tableView->setItem(nIndex, 1, pDisplay);

            ++nIndex;
        });
    }
}

void NodeView::Show(const xf::credential::account_t& account)
{
    if (xf::credential::account_t::item_type::type != _ct)
    {
        _ct = xf::credential::account_t::item_type::type;

        _ui.AdjustIndicator(3);
        _ui.AdjustLabel(3);

        const char* labels[3]{ "Last Update: ", "Account: ", "Description: " };
        for (unsigned int i = 0; i < 3; ++i)
            _ui.m_Displays[i].first->setText(labels[i]);
    }

    _ui.m_Displays[0].second->setText(QDateTime::fromTime_t(account.Item().Time()).toString("yyyy-MM-dd HH:mm:ss"));
    _ui.m_Displays[1].second->setText(ToQString(account.Item().name));
    _ui.m_Displays[2].second->setText(ToQString(account.Item().description));

    _ui.m_tableView->clearContents();

    if (unsigned int nRows = account.Size(); 0 < nRows)
    {
        _ui.m_tableView->setRowCount(nRows);
        _ui.m_tableView->setColumnCount(2);
        _ui.m_tableView->setHorizontalHeaderLabels({ "    Key    ", "        Value        " });

        unsigned int nIndex = 0;
        account.Traversal([this, &nIndex](const xf::credential::pair_t& pair) mutable {
            auto pKey = MakeTableItem(ToQString(pair.Item().key), { 255, 64, 0 }, &pair);
            auto pValue = MakeTableItem(ToQString(pair.Item().value), { 32, 160, 32 }, &pair);

            _ui.m_tableView->setItem(nIndex, 0, pKey);
            _ui.m_tableView->setItem(nIndex, 1, pValue);

            ++nIndex;
        });
    }
}

void NodeView::OnClickedEdit()
{
}

void NodeView::OnClickedRemove()
{
}

void NodeView::OnEdit()
{

}

void NodeView::OnAdd()
{

}

void NodeView::OnRemove()
{

}

void NodeView::OnMoveUp()
{

}

void NodeView::OnMoveDown()
{

}

void NodeView::OnMoveItem(QTableWidgetItem* pItem, int offset)
{
}

void NodeView::OnDoubleClicked(QTableWidgetItem* pItem)
{
}

void NodeView::OnSort(int nIndex, Qt::SortOrder order)
{

}

void NodeView::ui_type::AdjustIndicator(unsigned int nIndicator)
{
    for (unsigned int i = 0; i < _IndicatorSize; ++i)
    {
        if (i < nIndicator)
        {
            m_Indicators[i].first->show();
            m_Indicators[i].second->show();
        }
        else
        {
            m_Indicators[i].first->hide();
            m_Indicators[i].second->hide();
        }
    }
}

void NodeView::ui_type::AdjustLabel(unsigned int nDisplay)
{
    for (unsigned int i = 0; i < _FieldMaxSize; ++i)
    {
        if (i < nDisplay)
        {
            m_Displays[i].first->show();
            m_Displays[i].second->show();
        }
        else
        {
            m_Displays[i].first->hide();
            m_Displays[i].second->hide();
        }
    }
}

void NodeView::ui_type::SetupUI(NodeView* pView)
{
    pView->setObjectName("NodeView");
    SetBackgroundColor(pView, Qt::white);

    for (unsigned int i = 0; i < _IndicatorSize; ++i)
    {
        m_Indicators[i].first = MakeStaticLabel(pView, g_clrLabel);
        m_Indicators[i].second = MakeTransparentButton(pView);
    }

    for (unsigned int i = 0; i < _FieldMaxSize; ++i)
    {
        m_Displays[i].first = MakeStaticLabel(pView, g_clrLabel);
        m_Displays[i].second = MakeShowLine(pView);
    }

    m_btnEdit = MakeTransparentButton(pView);
    m_btnRemove = MakeTransparentButton(pView);
    m_tableView = new DropTable(pView);

    QHBoxLayout* layoutIndicator = new QHBoxLayout;
    layoutIndicator->setMargin(0);
    layoutIndicator->setSpacing(8);
    for (unsigned int i = 0; i < _IndicatorSize; ++i)
    {
        layoutIndicator->addWidget(m_Indicators[i].first);
        layoutIndicator->addWidget(m_Indicators[i].second);
    }
    layoutIndicator->addStretch(1);
    layoutIndicator->addWidget(m_btnEdit);
    layoutIndicator->addWidget(m_btnRemove);

    QVBoxLayout* layoutLabelLeft = new QVBoxLayout;
    QVBoxLayout* layoutEditRight = new QVBoxLayout;
    layoutLabelLeft->setMargin(0);
    layoutEditRight->setMargin(0);
    layoutLabelLeft->setSpacing(2);
    layoutEditRight->setSpacing(2);
    for (unsigned int i = 0; i < _FieldMaxSize; ++i)
    {
        layoutLabelLeft->addWidget(m_Displays[i].first);
        layoutEditRight->addWidget(m_Displays[i].second);
    }

    QHBoxLayout* phLayout = new QHBoxLayout;
    phLayout->setContentsMargins(0, 0, 0, 16);
    phLayout->setSpacing(0);
    phLayout->addLayout(layoutLabelLeft);
    phLayout->addLayout(layoutEditRight);

    QVBoxLayout* pvMainLayout = new QVBoxLayout;
    pvMainLayout->setMargin(0);
    pvMainLayout->setSpacing(0);
    pvMainLayout->addLayout(layoutIndicator);
    pvMainLayout->addLayout(phLayout);
    pvMainLayout->addWidget(m_tableView, 1);

    QHBoxLayout* phMainLayout = new QHBoxLayout;
    phMainLayout->setContentsMargins(4, 0, 4, 4);
    phMainLayout->setSpacing(0);
    phMainLayout->addStretch(1);
    phMainLayout->addLayout(pvMainLayout, 16);
    phMainLayout->addStretch(1);

    pView->setLayout(phMainLayout);

    m_btnEdit->setText("Edit");
    m_btnRemove->setText("Remove");

    RetranslateUI(pView);
}

void NodeView::ui_type::RetranslateUI(NodeView* pView) { }

/*
void CredentialView::UpdateTable(id_type id)
{
    for (int i = 0; i < _ui.m_tableView->rowCount(); ++i)
    {
        if (auto pItem = _ui.m_tableView->item(i, 0))
        {
            if (id == pItem->data(Qt::UserRole).toUInt())
            {
                m_Credential.PreorderAction([this, pItem, id, i](const bnb::platform_node& platform) {
                    if (platform.GetID() == id)
                    {
                        pItem->setText(To_QString(platform.GetData().GetName()));
                        _ui.m_tableView->item(i, 1)->setText(To_QString(platform.GetData().GetUrl()));
                        _ui.m_tableView->item(i, 2)->setText(To_QString(platform.GetData().GetComment()));
                        return true;
                    }

                    return false;
                });

                break;
            }
        }
    }
}
*/
QT_END_NAMESPACE
