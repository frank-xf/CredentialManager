
#include "DropTable.h"


#include "../cm_view_utils.h"

QT_BEGIN_NAMESPACE

DropTable::DropTable(QWidget * parent)
    : QTableWidget(parent)
{
    _ui.SetupUI(this);

    setMinimumSize(512, 128);
    setFocusPolicy(Qt::NoFocus);
    setContextMenuPolicy(Qt::CustomContextMenu);

    setShowGrid(true);
    setAcceptDrops(true);
    setDragEnabled(true);
    setDropIndicatorShown(false);
    setDragDropOverwriteMode(false);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDefaultDropAction(Qt::MoveAction);
    setDragDropMode(QAbstractItemView::InternalMove);
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setDefaultSectionSize(def_widget_h);
    verticalHeader()->setVisible(false);
    horizontalHeader()->setFixedHeight(def_widget_h);
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->setSectionsClickable(true);
    horizontalHeader()->setHighlightSections(false);
    horizontalHeader()->setFont(MakeFont());
    horizontalHeader()->setStyleSheet("QHeaderView::section{ background-color:#F0F0F0; color: #FF4080; }");

    setStyleSheet("QTableView::item:selected:!active{ background:#1080F0; color:white; }");

    QObject::connect(this, &QTableWidget::customContextMenuRequested, this, &DropTable::OnTableContextMenu);
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
                    Q_EMIT moveItem(item_target, offset);
                    event->accept();
                }
            }
        }

        return;
    }

    QTableWidget::dropEvent(event);
}

void DropTable::mouseDoubleClickEvent(QMouseEvent * event)
{
    if (auto pItem = itemAt(event->pos()))
    {
        Q_EMIT doubleClicked(pItem);

        event->accept();
        return;
    }

    QTableWidget::mouseDoubleClickEvent(event);
}

void DropTable::OnTableContextMenu(const QPoint& pos)
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

    tableMenu.addAction(_ui.m_actAdd);
    tableMenu.addSeparator();
    tableMenu.addAction(_ui.m_actEdit);
    tableMenu.addAction(_ui.m_actRemove);
    tableMenu.addSeparator();
    tableMenu.addAction(_ui.m_actMoveUp);
    tableMenu.addAction(_ui.m_actMoveDown);

    tableMenu.exec(QCursor::pos());
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
