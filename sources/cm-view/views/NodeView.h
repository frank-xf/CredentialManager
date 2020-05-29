#pragma once

QT_BEGIN_NAMESPACE

class QLineEdit;
class DropTable;
class QTableWidgetItem;

//------------------------------------------------------------------------------

class NodeView : public QWidget
{
public:

    struct ui_type
    {
    private:

        static constexpr unsigned int _IndicatorSize = static_cast<unsigned int>(xf::credential::credential_type::ct_max) - 1;
        static constexpr unsigned int _FieldMaxSize = 4;

    public:

        std::pair<QLabel*, QPushButton*> m_Indicators[_IndicatorSize];
        std::pair<QLabel*, QLineEdit*> m_Displays[_FieldMaxSize];

		QPushButton* m_btnEdit;
		QPushButton* m_btnRemove;

        DropTable* m_tableView;

        void AdjustIndicator(unsigned int nIndicator);
        void AdjustLabel(unsigned int nIndicator);

        void SetupUI(NodeView* pView);
        void RetranslateUI(NodeView* pView);
    };

    NodeView(QWidget* parent);

    void Show(const xf::credential::credential_t& credential);
    void Show(const xf::credential::platform_t& platform);
    void Show(const xf::credential::account_t& account);

    void OnClickedEdit();
    void OnClickedRemove();

    void OnEdit();
    void OnAdd();
    void OnRemove();
    void OnMoveUp();
    void OnMoveDown();
    void OnMoveItem(QTableWidgetItem* pItem, int offset);
    void OnDoubleClicked(QTableWidgetItem* pItem);
    void OnSort(int nIndex, Qt::SortOrder order);

private:

    xf::credential::credential_type _ct;
    const void* _data;

    ui_type _ui;

};	// class NodeView

QT_END_NAMESPACE
