#ifndef _bnb_Drop_Table_H_
#define _bnb_Drop_Table_H_

QT_BEGIN_NAMESPACE

class DropTable : public QTableWidget
{
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

public:

    DropTable(DelegateTableView* pDelegate = nullptr, QWidget * parent = nullptr);

private:

    void OnEdit();
    void OnAdd();
    void OnRemove();
    void OnMoveUp();
	void OnMoveDown();
	void OnSort(int nIndex, Qt::SortOrder order);
    void OnTableContextMenu(const QPoint & pos);

    void dropEvent(QDropEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:

    ui_type _ui;

    DelegateTableView* _delegate;
};

QT_END_NAMESPACE

#endif  // _bnb_Drop_Table_H_
