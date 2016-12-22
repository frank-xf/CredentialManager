#include <QtGui/QDropEvent>
#include <QtWidgets/QMenu>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QStyledItemDelegate>

#include "credential_qt_utils.h"

#include "Widget/NoFocusDelegate.h"
#include "Widget/DropTable.h"

static inline unsigned int GetItemID(const QTableWidgetItem* item)
{
    return item->data(Qt::UserRole).toUInt();
}

QT_BEGIN_NAMESPACE

DropTable::DropTable(delegate_type* pDelegate, QWidget * parent)
    : QTableWidget(parent)
    , _delegate(pDelegate)
{
    _ui.SetupUI(this);

    setItemDelegate(new NoFocusDelegate);
    setMinimumSize(512, 128);

    setShowGrid(true);
    setAcceptDrops(true);
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setDragDropOverwriteMode(false);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDefaultDropAction(Qt::MoveAction);
    setDragDropMode(QAbstractItemView::InternalMove);

    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setContextMenuPolicy(Qt::CustomContextMenu);

    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setDefaultSectionSize(ui_utils::def_widget_h);
    verticalHeader()->setVisible(false);
    horizontalHeader()->setFixedHeight(ui_utils::def_widget_h);
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->setSectionsClickable(true);
    horizontalHeader()->setHighlightSections(false);
    horizontalHeader()->setFont(ui_utils::MakeFont());
    horizontalHeader()->setStyleSheet("QHeaderView::section{ background-color:#F0F0F0; color: #FF4080; }");

    QObject::connect(this, &QTableWidget::customContextMenuRequested, this, &DropTable::OnTableContextMenu);

    QObject::connect(_ui.m_actAdd, &QAction::triggered, this, &DropTable::OnAdd);
    QObject::connect(_ui.m_actEdit, &QAction::triggered, this, &DropTable::OnEdit);
    QObject::connect(_ui.m_actRemove, &QAction::triggered, this, &DropTable::OnRemove);
    QObject::connect(_ui.m_actMoveUp, &QAction::triggered, this, &DropTable::OnMoveUp);
    QObject::connect(_ui.m_actMoveDown, &QAction::triggered, this, &DropTable::OnMoveDown);
    QObject::connect(horizontalHeader(), &QHeaderView::sortIndicatorChanged, this, &DropTable::OnSort);
}

void DropTable::OnEdit()
{
    if (auto pItem = currentItem())
        if (_delegate)
            _delegate->OnEdit(GetItemID(pItem));
}

void DropTable::OnAdd()
{
    if (_delegate)
        _delegate->OnAdd();
}

void DropTable::OnRemove()
{
    if (auto pItem = currentItem())
        if (_delegate)
            _delegate->OnRemove(GetItemID(pItem));
}

void DropTable::OnMoveUp()
{
    if (auto pItem = currentItem())
        if (_delegate)
            _delegate->OnMove(GetItemID(pItem), -1);
}

void DropTable::OnMoveDown()
{
    if (auto pItem = currentItem())
        if (_delegate)
            _delegate->OnMove(GetItemID(pItem), 1);
}

void DropTable::OnSort(int nIndex, Qt::SortOrder order)
{
    if (_delegate)
        _delegate->OnSort(nIndex, Qt::SortOrder::AscendingOrder == order);
}

void DropTable::OnTableContextMenu(const QPoint & pos)
{
    QMenu tableMenu(this);
    tableMenu.setStyleSheet("QMenu{ border: 1px solid gray; background-color: white; }"
        "QMenu::item{ padding: 2px 16px 2px 16px; }"
        "QMenu::item:selected{ background:#C0F0C0;}"
        "QMenu::separator{ height: 1px; background: gray; margin-left: 2px; margin-right: 2px;}");

    if (QTableWidgetItem* pItem = itemAt(pos))
    {
        int nIndex = row(pItem);
        _ui.m_actMoveUp->setEnabled(0 < nIndex);
        _ui.m_actMoveDown->setEnabled(nIndex < rowCount() - 1);

        _ui.m_actRemove->setEnabled(true);
        _ui.m_actEdit->setEnabled(true);
    }
    else
    {
        _ui.m_actRemove->setEnabled(false);
        _ui.m_actEdit->setEnabled(false);
        _ui.m_actMoveUp->setEnabled(false);
        _ui.m_actMoveDown->setEnabled(false);
    }

    tableMenu.addAction(_ui.m_actEdit);
    tableMenu.addAction(_ui.m_actRemove);
    tableMenu.addAction(_ui.m_actAdd);
    tableMenu.addSeparator();
    tableMenu.addAction(_ui.m_actMoveUp);
    tableMenu.addAction(_ui.m_actMoveDown);

    tableMenu.exec(QCursor::pos());
}

void DropTable::dropEvent(QDropEvent * event)
{
    if (this == event->source()
        && Qt::MoveAction == event->dropAction()
        && Qt::MoveAction == (Qt::MoveAction & event->possibleActions()))
    {
        auto listItem = selectedItems();
        if (!listItem.isEmpty())
        {
            QTableWidgetItem* item_target = listItem.at(0);
            QTableWidgetItem* item_insert = itemAt(event->pos());
            if (item_target && item_insert)
            {
                int offset = row(item_insert) - row(item_target);
                if (0 != offset)
                {
                    if (_delegate)
                    {
                        _delegate->OnMove(GetItemID(item_target), offset);

                        event->accept();
                    }
                }
            }
        }
    }
    else
    {
        QTableWidget::dropEvent(event);
    }
}

void DropTable::mouseDoubleClickEvent(QMouseEvent * event)
{
    if (auto pItem = itemAt(event->pos()))
    {
        if (_delegate)
            _delegate->OnEdit(GetItemID(pItem));

        event->accept();
        return;
    }

    QTableWidget::mouseDoubleClickEvent(event);
}

//------------------------------------------------------------------------------

void DropTable::ui_type::SetupUI(DropTable* pView)
{
    m_actAdd = new QAction(pView);
    m_actRemove = new QAction(pView);
    m_actEdit = new QAction(pView);
    m_actMoveUp = new QAction(pView);
    m_actMoveDown = new QAction(pView);

    RetranslateUI(pView);
}

void DropTable::ui_type::RetranslateUI(DropTable * pView)
{
    m_actAdd->setText("Add");
    m_actRemove->setText("Remove");
    m_actEdit->setText("Edit");
    m_actMoveUp->setText("Move Up");
    m_actMoveDown->setText("Move Down");
}

QT_END_NAMESPACE
