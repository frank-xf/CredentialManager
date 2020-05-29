#pragma once

#include <QtGui/QDropEvent>
#include <QtWidgets/QMenu>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class DropTable : public QTableWidget
{
    Q_OBJECT

public:

    struct ui_type
    {
        QAction* m_actAdd;
        QAction* m_actRemove;
        QAction* m_actEdit;
        QAction* m_actMoveUp;
        QAction* m_actMoveDown;

        void SetupUI(DropTable* pView);
        void RetranslateUI(DropTable* pView);
    };

    DropTable(QWidget* parent = nullptr);

    ui_type _ui;

private:

    void OnTableContextMenu(const QPoint& pos);

    void dropEvent(QDropEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

Q_SIGNALS:

    void moveItem(QTableWidgetItem*, int);
    void doubleClicked(QTableWidgetItem*);

};

QT_END_NAMESPACE

