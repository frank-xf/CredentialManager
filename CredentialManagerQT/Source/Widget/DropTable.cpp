#include <QtGui/QDropEvent>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QStyledItemDelegate>

#include "credential_qt_utils.h"

#include "Widget/NoFocusDelegate.h"
#include "Widget/DropTable.h"

#include <iostream>


QT_BEGIN_NAMESPACE

DropTable::DropTable(QWidget * parent)
    : QTableWidget(parent)
{
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
    horizontalHeader()->setSectionsClickable(false);
    horizontalHeader()->setHighlightSections(false);
    horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    horizontalHeader()->setFont(ui_utils::MakeFont());
    horizontalHeader()->setStyleSheet("QHeaderView::section{ background-color:#F0F0F0; color: #FF4080; }");
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

                }
            }
        }

        event->accept();
    }
    else
    {
        QTableWidget::dropEvent(event);
    }
}

QT_END_NAMESPACE
