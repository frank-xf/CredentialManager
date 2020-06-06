#pragma once

#include "../cm-core/cm_core.h"

QT_BEGIN_NAMESPACE

class ViewDelegate
{
private:

    ViewDelegate(const ViewDelegate&) = delete;
    ViewDelegate& operator=(const ViewDelegate&) = delete;

protected:

    ViewDelegate() = default;

    QString m_FilePath;
    xf::credential::credential_t m_Credential;

public:

    virtual bool OnLoad() = 0;
    virtual bool OnCreate() = 0;

    /*
    virtual bool OnAdd(xf::credential::credential_t* ptr) = 0;
    virtual bool OnAdd(xf::credential::platform_t* ptr) = 0;
    virtual bool OnAdd(xf::credential::account_t* ptr) = 0;

    virtual bool OnChangePassword(xf::credential::credential_t* ptr) = 0;
    virtual bool OnUpdate(xf::credential::credential_t* ptr) = 0;
    virtual bool OnUpdate(xf::credential::platform_t* ptr) = 0;
    virtual bool OnUpdate(xf::credential::account_t* ptr) = 0;
    virtual bool OnUpdate(xf::credential::pair_t* ptr) = 0;

    virtual bool OnRemove(xf::credential::credential_t* ptr) = 0;
    virtual bool OnRemove(xf::credential::platform_t* ptr) = 0;
    virtual bool OnRemove(xf::credential::account_t* ptr) = 0;
    virtual bool OnRemove(xf::credential::pair_t* ptr) = 0;

    virtual bool OnMove(xf::credential::platform_t* ptr, int offset) = 0;
    virtual bool OnMove(xf::credential::account_t* ptr, int offset) = 0;
    virtual bool OnMove(xf::credential::pair_t* ptr, int offset) = 0;
    */
    /*
	virtual bool OnSortPlatform(xf::credential::credential_t* ptr, int cln, bool ascending) = 0;
	virtual bool OnSortAccount(xf::credential::credential_t* ptr, int cln, bool ascending) = 0;
	virtual bool OnSortPair(xf::credential::credential_t* ptr, int cln, bool ascending) = 0;
    */
    /*
    virtual void SwitchToNode(const xf::credential::credential_t* ptr) = 0;
    virtual void SwitchToNode(const xf::credential::platform_t* ptr) = 0;
    virtual void SwitchToNode(const xf::credential::account_t* ptr) = 0;
    // virtual void SwitchToNode(xf::credential::pair_t* ptr) = 0;
    virtual void SwitchToView() = 0;
    */
    virtual ~ViewDelegate() { }

};

inline xf::credential::string_t FromQString(const QString& str)
{
    return str.toStdString();
}

inline QString ToQString(const xf::credential::string_t& str)
{
    return QString::fromStdString(str);
}

template<typename T>
inline QVariant PointerToQVariant(T* ptr)
{
    // return QVariant::fromValue(static_cast<std::uintptr_t>(ptr));
    return 0;
}

template<typename T>
inline T* QVariantToPointer(const QVariant& v)
{
    // return static_cast<T*>(v.value<std::uintptr_t>());
    return nullptr;
}

QT_END_NAMESPACE
