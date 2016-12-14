#ifndef _bnb_Tree_View_H_
#define _bnb_Tree_View_H_

QT_BEGIN_NAMESPACE

class TreeView : public QTreeWidget
{
private:
    
    void mouseDoubleClickEvent(QMouseEvent *event) override;

public:

	TreeView(QWidget * parent = nullptr);

	void ClearCredential();
	void UpdateHeader(const QString& strText);

    QTreeWidgetItem* AddRoot(const bnb::Credential& pc);
    QTreeWidgetItem* AddPlatform(QTreeWidgetItem* parent, const bnb::platform_node& pp);
	QTreeWidgetItem* AddAccount(QTreeWidgetItem* parent, const bnb::account_node& pa);
	QTreeWidgetItem* AddProperty(QTreeWidgetItem* parent, const bnb::property_node& pp);

    
};

QT_END_NAMESPACE

#endif	// _bnb_Tree_View_H_
