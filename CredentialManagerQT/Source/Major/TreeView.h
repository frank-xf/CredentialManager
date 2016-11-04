#ifndef _bnb_Tree_View_H_
#define _bnb_Tree_View_H_

class TreeView : public QTreeWidget {
	

public:

	TreeView(QWidget * parent = nullptr);

	void InitCredential();
	void ClearCredential();
	void UpdateHeader();

	QTreeWidgetItem* AddPlatform(const bnb::platform_type* pp);
	QTreeWidgetItem* AddAccount(QTreeWidgetItem* parent, const bnb::account_type* pa);
	QTreeWidgetItem* AddProperty(QTreeWidgetItem* parent, const bnb::property_type* pp);

private:
	
};

#endif	// _bnb_Tree_View_H_
