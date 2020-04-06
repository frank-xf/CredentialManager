#ifndef _bnb_Credential_QT_String_H_
#define _bnb_Credential_QT_String_H_

bnb::string_type From_QString(const QT_PREPEND_NAMESPACE(QString)& str);
QT_PREPEND_NAMESPACE(QString) To_QString(const bnb::string_type& str);

#endif  // _bnb_Credential_QT_String_H_
