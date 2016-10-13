#ifndef _bnb_Credential_Model_Manager_H_
#define _bnb_Credential_Model_Manager_H_

#include "Credential/Credential.h"

class CredentialModelManager
{
public:

    static CredentialModelManager& Instance()
    {
        static CredentialModelManager _x_;
        return (_x_);
    }

    void Initialize();

	const bnb::string_type& GetFile() const { return m_strFile; }
	void SetFile(const bnb::string_type& strFile) { m_strFile = strFile; }

	bnb::Credential& Info() { return m_Credential; }

	bool Load(const bnb::string_type& strFile);
	bool SaveCredential() const;

	bnb::platform_tree::data_ptr FindPlatform(const bnb::platform_type& platform);
	bnb::account_tree::data_ptr FindAccount(const bnb::platform_type& platform, const bnb::account_type& account);

private:

    CredentialModelManager() { }
    CredentialModelManager(const CredentialModelManager&) = delete;
    CredentialModelManager& operator=(const CredentialModelManager&) = delete;

	bnb::string_type m_strFile;
	bnb::Credential m_Credential;

};

#endif  // _bnb_Credential_Model_Manager_H_
