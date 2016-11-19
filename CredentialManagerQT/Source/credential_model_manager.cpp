#include <QtWidgets/QDialog>

#include "Credential/Credential.h"

#include "credential_model_manager.h"

void CredentialModelManager::Initialize()
{
}

bool CredentialModelManager::Load(const bnb::memory_type & source, const bnb::string_type& password)
{
	if (m_Credential.FromXml(source))
	{
        m_Credential.SetWord(password);
        return true;
	}

    return false;
}

bool CredentialModelManager::SaveCredential()
{
    m_Credential.UpdateTime();
	return m_Credential.Save(m_strFile.c_str());
}
