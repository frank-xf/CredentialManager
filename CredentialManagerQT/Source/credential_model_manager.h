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

	const QT_PREPEND_NAMESPACE(QString)& GetFile() const { return m_strFile; }
	void SetFile(const QT_PREPEND_NAMESPACE(QString)& strFile) { m_strFile = strFile; }

	bnb::Credential& Data() { return m_Credential; }

	bool SaveCredential();

private:

    CredentialModelManager() = default;
    CredentialModelManager(const CredentialModelManager&) = delete;
    CredentialModelManager& operator=(const CredentialModelManager&) = delete;

    QT_PREPEND_NAMESPACE(QString) m_strFile;
	bnb::Credential m_Credential;

};

#endif  // _bnb_Credential_Model_Manager_H_
