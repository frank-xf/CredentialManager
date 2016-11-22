#include <windows.h>

#include <QtCore/QString>

#include "Credential/Credential.h"

#include "credential_model_manager.h"

void UTF8toANSI(std::string &strUTF8)
{
    //获取转换为多字节后需要的缓冲区大小，创建多字节缓冲区  
    unsigned int nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8.c_str(), -1, NULL, NULL);
    wchar_t *wszBuffer = new wchar_t[nLen + 1];
    nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8.c_str(), -1, wszBuffer, nLen);
    wszBuffer[nLen] = 0;

    nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
    char *szBuffer = new char[nLen + 1];
    nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
    szBuffer[nLen] = 0;

    strUTF8 = szBuffer;
    //清理内存  
    delete[]szBuffer;
    delete[]wszBuffer;
}

void CredentialModelManager::Initialize()
{
}

bool CredentialModelManager::SaveCredential()
{
    m_Credential.UpdateTime();

    bnb::memory_type xml;

    m_Credential.ToXml(xml);
    std::string _xml((char*)xml.c_str(), xml.size());
    UTF8toANSI(_xml);

    printf(_xml.c_str());

	return m_Credential.Save(m_strFile.toStdString().c_str());
}

bnb::string_type From_QString(const QString& strText) { return strText.toStdWString(); }
QString To_QString(const bnb::string_type& str) { return QString::fromStdWString(str); }