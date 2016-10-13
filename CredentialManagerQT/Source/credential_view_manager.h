#ifndef _bnb_Credential_View_Manager_H_
#define _bnb_Credential_View_Manager_H_

class QWidget;

class CredentialViewManager
{
public:

    static CredentialViewManager& Instance()
	{
        static CredentialViewManager _x_;
        return (_x_);
    }

    void Initialize();

	QWidget* Main();

private:

    CredentialViewManager() { }
    CredentialViewManager(const CredentialViewManager&) = delete;
    CredentialViewManager& operator=(const CredentialViewManager&) = delete;

	QWidget* m_viewMain;
};

#endif  // _bnb_Credential_View_Manager_H_
