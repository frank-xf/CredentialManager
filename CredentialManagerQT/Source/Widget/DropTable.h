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

    class delegate_type
    {
    private:

        delegate_type(const delegate_type&) = delete;
        delegate_type& operator=(const delegate_type&) = delete;

    protected:

        delegate_type() = default;

    public:

        virtual void OnAdd() = 0;
        virtual void OnEdit(unsigned int id) = 0;
        virtual void OnRemove(unsigned int id) = 0;
        virtual void OnMove(unsigned int id, int offset) = 0;
        virtual void OnSort(int cln, bool ascending) = 0;
        virtual void OnDoubleClicked(unsigned int id) = 0;

        virtual ~delegate_type() = 0 { }

    };

    DropTable(delegate_type* pDelegate = nullptr, QWidget * parent = nullptr);

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

    delegate_type* _delegate;
};

QT_END_NAMESPACE

#endif  // _bnb_Drop_Table_H_
