#ifndef _bnb_Content_View_H_
#define _bnb_Content_View_H_

class ContentView : public QStackedWidget
{
public:

	ContentView(QWidget * parent = nullptr);

	void InitCredential();
	void ClearCredential();

	void UpdatePlatform(unsigned int id);
	void UpdateAccount(unsigned int id);

	bool SwitchToCredential(unsigned int id);
	bool SwitchToPlatform(unsigned int id);
	bool SwitchToAccount(unsigned int id);
	bool SwitchToView(unsigned int id);
	bool SwitchToHint();
	bool SwitchToHint(const QString& strText);


private:

	QLabel* m_labHint;
};

#endif  // _bnb_Content_View_H_
