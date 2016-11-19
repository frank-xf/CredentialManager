#ifndef _bnb_Credential_View_Manager_H_
#define _bnb_Credential_View_Manager_H_

QT_BEGIN_NAMESPACE

class QWidget;

QT_END_NAMESPACE

class CredentialViewManager
{
public:

    static CredentialViewManager& Instance()
	{
        static CredentialViewManager _x_;
        return (_x_);
    }

    void Initialize();

private:

    CredentialViewManager() = default;
    CredentialViewManager(const CredentialViewManager&) = delete;
    CredentialViewManager& operator=(const CredentialViewManager&) = delete;

    QT_PREPEND_NAMESPACE(QWidget)* m_viewMain;
};

#endif  // _bnb_Credential_View_Manager_H_
