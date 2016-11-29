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
                QRect itemRect = visualItemRect(item_insert);
                QAbstractItemView::AboveItem
            }


        }

        event->accept();
    }
    else
    {
        QTableWidget::dropEvent(event);
    }

    if (auto pItem = itemAt(event->pos()))
    {
        std::cout << "text: " << pItem->text().toStdString() << std::endl;
        std::cout << "row: " << row(pItem) << std::endl;
        std::cout << "column: " << column(pItem) << std::endl;
    }

    std::cout << "mouse: " << event->mouseButtons() << std::endl;
    std::cout << "possibleActions: " << event->possibleActions() << std::endl;
    std::cout << "proposedAction: " << event->proposedAction() << std::endl;
    std::cout << "dropAction: " << event->dropAction() << std::endl;
    std::cout << "is this: " << ((event->source() == this) ? "true" : "false") << std::endl;
    std::cout << "---------------------------------" << std::endl;

    int i = 0;
    auto listItem = selectedItems();
    for (auto pItem : listItem)
    {
        if (pItem)
        {
            std::cout << i <<" text: " << pItem->text().toStdString() << ", ("<< row(pItem) <<", "<< column(pItem) <<")"<<std::endl;
        }
        ++i;
    }

    event->accept();
}

QT_END_NAMESPACE
