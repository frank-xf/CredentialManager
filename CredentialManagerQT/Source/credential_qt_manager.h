#ifndef _bnb_Credential_QT_Manager_H_
#define _bnb_Credential_QT_Manager_H_

class CredentialViewManager;
class CredentialModelManager;

class CredentialManagerQT
{
public:

    static CredentialManagerQT& Instance()
	{
        static CredentialManagerQT _x_;
        return (_x_);
    }

    void Initialize();

	CredentialViewManager& View();
	CredentialModelManager& Model();

private:

    CredentialManagerQT() { };
    CredentialManagerQT(const CredentialManagerQT&) = delete;
    CredentialManagerQT& operator=(const CredentialManagerQT&) = delete;

};

#define g_AppMgr    CredentialManagerQT::Instance()

#endif  // _bnb_Credential_QT_Manager_H_
