#ifndef _bnb_Tree_View_H_
#define _bnb_Tree_View_H_

QT_BEGIN_NAMESPACE

class TreeView : public QTreeWidget
{
private:
    
    void mouseDoubleClickEvent(QMouseEvent *event) override;

public:

	TreeView(QWidget * parent = nullptr);

    void InitCredential(const bnb::Credential& credential);
	void ClearCredential();
	void UpdateHeader(const QString& strText);

    QTreeWidgetItem* AddPlatform(const bnb::platform_node& pp);
	QTreeWidgetItem* AddAccount(const bnb::account_node& pa);
	QTreeWidgetItem* AddProperty(const bnb::property_node& pp);

    QTreeWidgetItem* AddPlatform(QTreeWidgetItem * parent, const bnb::platform_node& pp);
    QTreeWidgetItem* AddAccount(QTreeWidgetItem * parent, const bnb::account_node& pa);
    QTreeWidgetItem* AddProperty(QTreeWidgetItem * parent, const bnb::property_node& pp);

    QTreeWidgetItem* FindItem(unsigned int id);
    QTreeWidgetItem* FindItem(QTreeWidgetItem* parent, unsigned int id);
};

QT_END_NAMESPACE

#endif	// _bnb_Tree_View_H_
