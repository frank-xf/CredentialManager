#ifndef _bnb_Credential_Model_Manager_H_
#define _bnb_Credential_Model_Manager_H_

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

	bnb::Credential& Data() { return m_Credential; }

	bool Load(const bnb::memory_type & source, const bnb::string_type& password);
	bool SaveCredential();

private:

    CredentialModelManager() { }
    CredentialModelManager(const CredentialModelManager&) = delete;
    CredentialModelManager& operator=(const CredentialModelManager&) = delete;

	bnb::string_type m_strFile;
	bnb::Credential m_Credential;

};

#endif  // _bnb_Credential_Model_Manager_H_
