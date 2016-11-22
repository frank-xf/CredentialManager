#ifndef _bnb_Credential_QT_String_H_
#define _bnb_Credential_QT_String_H_

QT_BEGIN_NAMESPACE

bnb::string_type From_QString(const QString& strText);
QString To_QString(const bnb::string_type& str);

QT_END_NAMESPACE

#endif  // _bnb_Credential_QT_String_H_
