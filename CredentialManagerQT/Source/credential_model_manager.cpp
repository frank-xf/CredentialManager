#include <QtWidgets/QDialog>

#include "credential_qt_manager.h"
#include "credential_view_manager.h"
#include "credential_model_manager.h"

#include "Dialog/HintDialog.h"
#include "Dialog/PasswordInput.h"

void CredentialModelManager::Initialize()
{
}

bool CredentialModelManager::Load(const bnb::string_type & strFile)
{
	bnb::memory_type dst;

	if (!bnb::Credential::CheckFile(strFile.c_str(), &dst))
	{
		HintDialog(hint_type::ht_error, "You selected file invalid !", g_AppMgr.View().Main()).exec();
		return false;
	}

	PasswordInput dlg(g_AppMgr.View().Main());
	if (QDialog::Accepted != dlg.exec())
	{
		return false;
	}

	QString password = dlg.GetPassword();
	if (!bnb::Credential::Decoding(dst, (const unsigned char*)password.toStdString().c_str(), password.size()))
	{
		HintDialog(hint_type::ht_error, "You input password error !", g_AppMgr.View().Main()).exec();
		return false;
	}

	if (!m_Credential.FromXml(dst))
	{
		HintDialog(hint_type::ht_error, "Anaylze file failed !", g_AppMgr.View().Main()).exec();
		return false;
	}

	m_Credential.SetWord(dlg.GetPassword().toStdString());

	return true;
}

bool CredentialModelManager::SaveCredential() const
{
	return m_Credential.Save(m_strFile.c_str());
}

