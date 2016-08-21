#ifndef _bnb_Credential_View_H_
#define _bnb_Credential_View_H_


namespace bnb
{
    class Credential;
}

QT_BEGIN_NAMESPACE

class CredentialView : public QWidget
{
    struct _ui_element
    {
        QLabel* m_labPlatform;
        std::list<QLabel*> m_listAccount;
    };

public:
	
    CredentialView(QWidget * parent, const bnb::Credential& src);

    void MakeLabelList(const bnb::Credential& src);
    void LayoutView();

private:

    std::list<_ui_element> m_listLabel;

};

QT_END_NAMESPACE

#endif // !_bnb_Credential_View_H_
