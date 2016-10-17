#ifndef _bnb_Content_View_H_
#define _bnb_Content_View_H_

class ContentView : public QStackedWidget
{
	struct ui_type
	{
		QHBoxLayout* m_layMain;
	};

public:
	ContentView(QWidget * parent = nullptr);

	void InitCredential();
	void ClearCredential();

	void UpdatePlatform(const QString& sp) const;
	void UpdateAccount(const QString& sp, const QString& sa);

	bool SwitchToCredential();
	bool SwitchToPlatform(const QString& sp);
	bool SwitchToAccount(const QString& sp, const QString& sa);
	bool SwitchToHint();
	bool SwitchToHint(const QString& strText);


private:
	
};

#endif  // _bnb_Content_View_H_
